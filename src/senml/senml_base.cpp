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
 * base class for all senml objects
 */

#include "senml_base.h"
#include "senml_pack.h"

SenMLBase::SenMLBase() : _next(NULL), _prev(NULL) {
}

SenMLBase::~SenMLBase() {
    if (this->_prev) {
        this->_prev->setNext(this->_next);
    }
    if (this->_next) {
        this->_next->setPrev(this->_prev);
    }
}

void SenMLBase::setNext(SenMLBase *value) {
    if (value == NULL) { // if next becomes null and there is a root, then this object became the last in the list, so
                         // let the root know.
        SenMLBasePack *root = (SenMLBasePack *)this->getRoot();
        if (root) {
            root->setLast(*this);
        }
    }
    this->_next = value;
}

void SenMLBase::setPrev(SenMLBase *value) {
    this->_prev = value;
}

SenMLBase *SenMLBase::getPrev() {
    return this->_prev;
}

SenMLBase *SenMLBase::getRoot() {
    SenMLBase *prev = this->_prev;
    while (prev) {
        SenMLBase *newPrev = prev->getPrev();
        if (newPrev == NULL) {
            return prev;
        } else {
            prev = newPrev;
        }
    }
    return this; // if there was no first prev, it means we are root.
}
