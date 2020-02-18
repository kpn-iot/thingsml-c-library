/*  _  __  ____    _   _
 * | |/ / |  _ \  | \ | |
 * | ' /  | |_) | |  \| |
 * | . \  |  __/  | |\  |
 * |_|\_\ |_|     |_| \_|
 *
 * (c) 2020 KPN
 * License: MIT License.
 * Author: Joseph Verburg, Jan Bogaerts 
 *
 * pack (document) without base values
 */
#include <math.h>

#include "../cbor/cbor.h"
#include "senml_JsonStreamingParser.h"
#include "senml_basepack.h"
#include "senml_cbor_parser.h"
#include "senml_helpers.h"
#include "senml_json_parser.h"
#include "senml_logging.h"
#include <string.h>

void SenMLBasePack::setLast(SenMLBase &value) {
    // if we become the last item in the list, then the list is empty.
    if (&value == this) {
        this->_end = NULL;
    } else {
        this->_end = &value;
    }
}

bool SenMLBasePack::add(SenMLBase &item) {
    if (item.getNext() != NULL) {
        log_debug(F("already in list"));
        return false;
    }

    SenMLBase *last = this->_end;
    if (last) {
        last->setNext(&item);
        item.setPrev(last);
    } else {
        this->_start = &item;
        item.setPrev(this);
    }
    this->_end = &item;
    return true;
}

bool SenMLBasePack::clear() {
    SenMLBase *item = this->_start;
    while (item) {
        // if it's a pack element, it also needs to clear out it's children.
        if (item->isPack()) {
            ((SenMLBasePack *)item)->clear();
        }
        item->setPrev(NULL);
        SenMLBase *next = item->getNext();
        item->setNext(NULL);
        item = next;
    }
    this->setNext(NULL);
    this->setPrev(NULL);
    this->_end = NULL;
    this->_start = NULL;
    return true;
}

void SenMLBasePack::fromJson(Stream &source, SenMLStreamMethod format) {
    JsonStreamingParser parser;
    SenMLJsonListener listener(this);

    parser.setListener(&listener);
    char data;
    if (format == SENML_RAW) {
#ifdef __MBED__
        data = source.getc();
#else
        data = source.read();
#endif
    } else {
        data = readHexChar(&source);
    }

    while (data != -1) {
        parser.parse(data);
        if (format == SENML_RAW) {
#ifdef __MBED__
            data = source.getc();
#else
            data = source.read();
#endif
        } else {
            data = readHexChar(&source);
        }
    }
    // when we get here, all the data is stored in the document and callbacks have been called.
}

void SenMLBasePack::fromJson(const char *source) {
    JsonStreamingParser parser;
    SenMLJsonListener listener(this);

    parser.setListener(&listener);
    for (int i = 0; source[i] != 0; i++) {
        parser.parse(source[i]);
    }
    // when we get here, all the data is stored in the document and callbacks have been called.
}

void SenMLBasePack::fromCbor(Stream &source, SenMLStreamMethod format) {
    SenMLCborParser parser(this, format);
    parser.parse(&source);
}

void SenMLBasePack::fromCbor(char *source, int length, SenMLStreamMethod format) {
    SenMLCborParser parser(this, format);
    parser.parse(source, length);
}

int SenMLBasePack::toJson(Stream &dest, SenMLStreamMethod format) {
    StreamContext renderTo; // set up the global record that configures the rendering. This saves us some bytes on the
                            // stack and in code by not having to pass along the values as function arguments.
    _streamCtx = &renderTo;
    this->setupStreamCtx(&dest, format);
    return this->internalToJson();
}

int SenMLBasePack::toJson(char *dest, int length, SenMLStreamMethod format) {
    StreamContext renderTo; // set up the global record that configures the rendering. This saves us some bytes on the
                            // stack and in code by not having to pass along the values as function arguments.
    _streamCtx = &renderTo;
    this->setupStreamCtx(dest, length, format);
    return this->internalToJson();
}

// render the content of the current object to json data (string)
int SenMLBasePack::internalToJson() {
    printText(F("["), 1);
    int res = this->contentToJson();
    printText(F("]"), 1);
    return res + 2;
}

int SenMLBasePack::contentToJson() {
    int res = 1;
    printText(F("{"), 1);
    res += this->fieldsToJson();
    SenMLBase *next = this->_start;
    // we can only inline the first record. If the first item is a Pack (child device), then don't inline it.
    if (next && next->isPack() == false) {
        // Only add comma if we added fields from the pack
        if (this->getFieldLength() > next->getFieldLength()) {
            printText(F(","), 1);
            res++;
        }
        res += next->fieldsToJson();
        next = next->getNext();
    }
    printText(F("}"), 1);
    res++;
    while (next) {
        printText(F(","), 1);
        res++;
        res += next->contentToJson();
        next = next->getNext();
    }
    return res;
}

int SenMLBasePack::contentToCbor() {
    int res = cbor_serialize_map(this->getFieldLength());

    res += this->fieldsToCbor();
    SenMLBase *next = this->_start;
    if (next && next->isPack() == false) { // we can only inline the first record. If the first item is a Pack (child
                                           // device), then don't inline it.
        res += next->fieldsToCbor();
        next = next->getNext();
    }

    while (next) {
        res += next->contentToCbor();
        next = next->getNext();
    }
    return res;
}

void SenMLBasePack::setupStreamCtx(char *dest, int length, SenMLStreamMethod format) {
    _streamCtx->data.blob.data = dest;
    _streamCtx->data.blob.length = length;
    _streamCtx->data.blob.curPos = 0;
    _streamCtx->dataAsBlob = true;
    _streamCtx->format = format;
    _streamCtx->baseValue.baseUint = 0; // by default, there is no base value or sum
    _streamCtx->baseSum.baseUint = 0;
    _streamCtx->baseDataType = CBOR_TYPE_DATA; // data never adjusts for basevalue, so this is safe.
}

void SenMLBasePack::setupStreamCtx(Stream *dest, SenMLStreamMethod format) {
    _streamCtx->data.stream = dest;
    _streamCtx->format = format;
    _streamCtx->dataAsBlob = false;
    _streamCtx->baseValue.baseUint = 0; // by default, there is no base value or sum
    _streamCtx->baseSum.baseUint = 0;
    _streamCtx->baseDataType = CBOR_TYPE_DATA; // data never adjusts for basevalue, so this is safe.
}

int SenMLBasePack::toCbor(Stream &dest, SenMLStreamMethod format) {
    StreamContext renderTo; // set up the global record that configures the rendering. This saves us some bytes on the
                            // stack and in code by not having to pass along the values as function arguments.
    _streamCtx = &renderTo;
    this->setupStreamCtx(&dest, format);
    int res = cbor_serialize_array(this->getArrayLength());
    res += this->contentToCbor();
    return res;
}

int SenMLBasePack::toCbor(char *dest, int length, SenMLStreamMethod format) {
    StreamContext renderTo; // set up the global record that configures the rendering. This saves us some bytes on the
                            // stack and in code by not having to pass along the values as function arguments.
    _streamCtx = &renderTo;
    this->setupStreamCtx(dest, length, format);
    int res = cbor_serialize_array(this->getArrayLength());
    res += this->contentToCbor();
    return res;
}

int SenMLBasePack::getArrayLength() {
    int result = 0; // init to 0 cause if there is a record, the first will become part of the first element in the
                    // array, if we were to init to 1, we would have 1 record too many.
    SenMLBase *next = this->_start;
    while (next) { // we can only inline the first record. If the first item is a Pack (child device), then don't inline
                   // it.
        result += next->getArrayLength(); // custom record implementations may wrap multiple records.
        next = next->getNext();
    }
    // if there are no items in this pack, then we still render 1 array element, that of the pack itself.
    if (result == 0) {
        result = 1;
    }
    return result;
}