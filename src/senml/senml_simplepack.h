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
 * Simple pack without any base fields, use this when you do not need base fields and want a more efficient memory footprint (saves ~50 bytes).
 * 
 */
#ifndef THINGSML_SENML_SIMPLE_PACK
#define THINGSML_SENML_SIMPLE_PACK

#include "senml_basepack.h"

class SenMLSimplePack : public SenMLBasePack {

  public:
    SenMLSimplePack(){};
    ~SenMLSimplePack(){};

    int fieldsToJson(){
        return 0;
    };
    int fieldsToCbor() {
        return 0;
    };

    int getFieldLength() {
        SenMLBase *first = this->getFirst();
        // we can only inline the first record. If the first item is a Pack (child device), then don't inline it.
        if (first && first->isPack() == false) {
            return first->getFieldLength();
        }
        return 0;
    }
};

#endif // !THINGSML_SENML_SIMPLE_PACK