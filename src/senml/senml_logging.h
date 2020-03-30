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

#ifndef THINGSML_SENML_LOGGING
#define THINGSML_SENML_LOGGING

#include "defaults.h"

// contains a referenc to the global logger object assigned to the module
extern Stream *_senml_logger;

/**
 * Assign a stream to the library that can be used to write log information to.
 * @param logger a stream object that will be used for logging.
 * @returns none
 */
void senMLSetLogger(Stream &logger);

#ifndef log_debug
#ifdef __MBED__
#define log_debug(...)                                                                                                 \
    if (_senml_logger) {                                                                                               \
        _senml_logger->printf(__VA_ARGS__);                                                                            \
        _senml_logger->printf("\n\r");                                                                                 \
    }
#else
#define log_debug(...)                                                                                                 \
    if (_senml_logger)                                                                                                 \
    _senml_logger->println(__VA_ARGS__)
#endif
#endif

#endif // THINGSML_SENML_LOGGING
