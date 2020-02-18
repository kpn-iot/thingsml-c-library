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
 * logging
 */

#include "senml_logging.h"

Stream *_senml_logger = NULL;

void senMLSetLogger(Stream &logger) {
    _senml_logger = &logger;
};
