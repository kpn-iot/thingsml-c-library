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
 * base class for all senml objects header
 */

#ifndef THINGSML_SENML_BASE
#define THINGSML_SENML_BASE

#include "defaults.h"
#include "senml_enums.h"
#include <math.h>

/**
 * the base class for all objects that can be used in the senml data tree.
 */
class SenMLBase {
    friend class SenMLBasePack;
    friend class SenMLPack;
    friend class SenMLSimplePack;
    friend class SenMLRecord;
    friend class SenMLJsonListener;
    friend class SenMLBaseParser;

  public:
    SenMLBase();
    ~SenMLBase();

    /** get the next item in the list.
     * @returns: a pointer to the next SenMLBase object in the list or NULL when at the end of the list.
     */
    SenMLBase *getNext() {
        return this->_next;
    };

    /**
     * Get the root object of this list. Usually, this is a SenMLPack object.
     * The root object is defined as the first item in the list.
     * @returns: a pointer to the first SenMLBase object in the list or NULL when there is none.
     */
    SenMLBase *getRoot();

    // This function is called by the root SenMLPack object to indicate that the object
    // should adjust it's time info relative to the new base time (if applicable)
    // doesn't do anything by default
    // params: prev: previous base time
    //        time: new base time
    virtual void adjustToBaseTime(double prev, double time){};

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

  protected:
    /*
    renders all the fields to json, with the starting and ending brackets.
    Inheriters can extend this function if they want to add extra fields to the json output
    note: tihs is public so that custom implementations for the record object can use other objects internally and
    render to json using this function (ex: coordinatesRecord using 3 floatRecrods for lat, lon & alt.
    */
    virtual int contentToJson() = 0;

    // assign the element in the list that this object points to.
    void setNext(SenMLBase *value);

    // assign the previous element in the list that thisobject points to.
    void setPrev(SenMLBase *value);
    // assign the previous element in the list that thisobject points to.
    SenMLBase *getPrev();

    // derived classes can use this function to see if the root object (getRoot) is a SenMLPack
    // class or not.
    virtual bool isPack() {
        return false;
    }

    // renders the content of the pack object without [], but still with {} for objects
    virtual int contentToCbor() = 0;

    // calculates the nr of items that this object will put in the json array in senml representation
    // this is used for rendering cbor which needs to declare the nr of elements in an array.
    // packs can have multiple elements in the array, but also custom implementations for records that wrap muliple
    // records.
    virtual int getArrayLength() {
        return 1;
    };

    // calculates the nr of fields that this object will produce.
    virtual int getFieldLength() = 0;

  private:
    SenMLBase *_next; // reference to the next element in the list.
    SenMLBase *_prev; // reference to the previous element, needed for deletion of record
};

#endif // THINGSML_SENML_BASE
