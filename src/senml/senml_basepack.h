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
 * pack (document) without base values headers
 */

#ifndef THINGSML_SENML_BASE_PACK
#define THINGSML_SENML_BASE_PACK

#include "defaults.h"
#include "senml_base.h"

/**
 * SenMLBasePack represents a single senml document that can be sent or received.
 *
 */
class SenMLBasePack : public SenMLBase {
    friend class SenMLJsonListener;
    friend class SenMLCborParser;
    friend class SenMLPack;
    friend class SenMLBase;

  public:
    SenMLBasePack(){};
    ~SenMLBasePack(){};
    /**
     * render the content of the current object to json data (string).
     * This function is ideal for devices with low memory usage but offers less control over the rendering process.
     * @param dest the destination stream to where the data will be rendered without buffering it in memory
     * @param format determins how the data will be rendered. See SenMLStreamMethod for possible methods.
     * @returns nr of bytes that were rendered
     */
    int toJson(Stream &dest, SenMLStreamMethod format = SENML_RAW);

    /**
     * render the content of the current object to json data (string).
     * This function renders the data to a memory buffer. If the buffer is full before the entire object is
     * rendered, an error will be written to the debug stream.
     * @param dest a memory buffer to which the data will be rendred.
     * @param length the length of the memory buffer.
     * @param format determins how the data will be rendered. See SenMLStreamMethod for possible methods.
     * @returns nr of bytes that were rendered
     */
    int toJson(char *dest, int length, SenMLStreamMethod format = SENML_RAW);

    /**
     * render the content of the current object to cbor data (binary).
     * This function is ideal for devices with low memory usage but offers less control over the rendering process.
     * @param dest the destination stream to where the data will be rendered without buffering it in memory
     * @param format determins how the data will be rendered. See SenMLStreamMethod for possible methods.
     * @returns nr of bytes that were rendered
     */
    int toCbor(Stream &dest, SenMLStreamMethod format = SENML_RAW);

    /**
     * render the content of the current object to cbor data (binary).
     * This function renders the data to a memory buffer. If the buffer is full before the entire object is
     * rendered, an error will be written to the debug stream.
     * @param dest a memory buffer to which the data will be rendred.
     * @param length the length of the memory buffer.
     * @param format determins how the data will be rendered. See SenMLStreamMethod for possible methods.
     * @returns nr of bytes that were rendered
     */
    int toCbor(char *dest, int length, SenMLStreamMethod format = SENML_RAW);

    /**
     * read and parse a senml json string from the specified source and, for each registered actuator, call the
     * appropriate event on the actuator itself, for others, the callback function PACK_ACTUATOR_SIGNATURE
     * will be called, if present.
     * This method is ideal for devices with very littel ram memory. It will block on most devices if there is
     * no more input to be read from the stream and the end of the json structure is not yet reached.
     * Note: on mbed systems, the blocking nature is not garanteed. Instead, if no more data is available before
     * the end is reached, parsing will fail.
     * @param source the source stream to read the data from.
     * @param format determins how the data will be read (ex: as normal text or in HEX format).
     *               See SenMLStreamMethod for possible methods.
     * @returns none
     */
    void fromJson(Stream &source, SenMLStreamMethod format = SENML_RAW);

    /**
     * parse a senml json string from the specified source and, for each registered actuator, call the
     * appropriate event on the actuator itself, for others, the callback function PACK_ACTUATOR_SIGNATURE
     * will be called, if present.
     * This method takes a string stored in memory as input. The json must be fully defined. It is up to the
     * caller to transform it to a regular text string, if needed (ex: lora devices might send it in hex format).
     * @param source the source string to use as input. This must be null terminated.
     * @returns none
     */
    void fromJson(const char *source);

    /**
     * read and parse senml cbor from the specified source and, for each registered actuator, call the
     * appropriate event on the actuator itself, for others, the callback function PACK_ACTUATOR_SIGNATURE
     * will be called, if present.
     * This method is ideal for devices with very littel ram memory. It will block on most decices if there is no
     * more input to be read from the stream and the end of the cbor structure is not yet reached.
     * Note: on mbed systems, the blocking nature is not garanteed. Instead, if no more data is available before
     * the end is reached, parsing will fail.
     * @param source the source stream to read the data from.
     * @param format determins how the data will be read (ex: as normal binary or in HEX format).
     *               See SenMLStreamMethod for possible methods.
     * @returns none
     */
    void fromCbor(Stream &source, SenMLStreamMethod format = SENML_RAW);

    /**
     * parse senml cbor from the specified memory and, for each registered actuator, call the
     * appropriate event on the actuator itself, for others, the callback function PACK_ACTUATOR_SIGNATURE
     * will be called, if present.
     * This method takes a memory blob as input. The data must be fully defined.
     * @param source the source data to use as input.
     * @param length the length of the source data.
     * @param format determins how the data will be read (ex: as normal binary or in HEX format).
     *               See SenMLStreamMethod for possible methods.
     * @returns none
     */
    void fromCbor(char *source, int length, SenMLStreamMethod format);

    /**
     * Adds the specified SenML object to the document. The item will be appended to the end of the linked list.
     * The item being added, can be a regular SenMLRecord or another SenMLPack object if you want to send data
     * for multiple devices in 1 SenML message.
     * Check the result of the function to see if the operation was successful or not. Possible reasons for failure:
     * - if the item being added is already part of a document.
     * @param item a pointer to a SenMlRecord or SenMLPack that needs to be added to the document.
     * @returns true upon success, otherwise false.
     */
    bool add(SenMLBase &item);

    /**
     * Clear out the document and remove all the children. Children aren't destroyed, this is up to the developer.
     * @returns true (at the moment, the function does not yet return false as it doesn't detect any errors)
     */
    bool clear();

    /**
     * get the first recrod of in this pack element.
     * @returns null when this object is empty (has no children), otherwise, the first item (SenMLRecord or SenMLPack)
     * of the list.
     */
    inline SenMLBase *getFirst() {
        return this->_start;
    };

    /**
     * @returns the number of items this pack contains.
     */
    virtual int getCount();

    /**
     * renders all the fields to json, without the starting and ending brackets.
     * Inheriters can extend this function if they want to add extra fields to the json output
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 floatRecrods for lat, lon & alt.
     * @returns: None
     */
    virtual int fieldsToJson() = 0;

    /**
     * renders all the fields to cbor format. renders all the fields of the object without the length info
     * at the beginning
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 floatRecrods for
     * lat, lon & alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToCbor() = 0;

    virtual const char *getBaseName() {
        return "";
    };

    virtual inline SenMLUnit getBaseUnit() {
        return SENML_UNIT_NONE;
    };

  protected:
    // derived classes can use this function to see if the root object (getRoot) is a SenMLPack
    // class or not.
    virtual bool isPack() {
        return true;
    }

    virtual bool isActuator() {
        return false;
    }

    // store a ref to the last item in the list for quick link operations
    void setLast(SenMLBase &value);

    // renders the content of the pack object without []
    virtual int contentToCbor();

    virtual int contentToJson();

    // calculates the nr of items that there will be in the json array in senml representation
    // this is used for rendering cbor which needs to declare the nr of elements in an array.
    virtual int getArrayLength();

    virtual int getFieldLength() {
        return 0;
    };

    virtual void setupStreamCtx(char *dest, int length, SenMLStreamMethod format);

    virtual void setupStreamCtx(Stream *dest, SenMLStreamMethod format);

  private:
    SenMLBase *_end = NULL;   // keeps track of the end of the list
    SenMLBase *_start = NULL; // keeps track of the start of the list

    int internalToJson();

    inline char readHexChar(Stream *source) {
#ifdef __MBED__
        unsigned char first = source->getc();
        unsigned char second = source->getc();
#else
        unsigned char first = source->read();
        unsigned char second = source->read();
#endif
        first = (first < '9') ? first - '0' : first - '7';
        second = (second < '9') ? second - '0' : second - '7';
        return (16 * first) + second;
    };
};

#endif // THINGSML_SENML_BASE_PACK
