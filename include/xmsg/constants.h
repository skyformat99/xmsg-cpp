/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */
/*
 * Copyright (C) 2015. Jefferson Lab, xMsg framework (JLAB). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and not-for-profit purposes,
 * without fee and without a signed licensing agreement.
 *
 * Contact Vardan Gyurjyan
 * Department of Experimental Nuclear Physics, Jefferson Lab.
 *
 * IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF
 * THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE CLARA SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 * HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef XMSG_CORE_CONSTANTS_H_
#define XMSG_CORE_CONSTANTS_H_

#include <string>

namespace xmsg {

namespace constants {

const std::string undefined = "undefined";
const std::string success = "success";
const std::string any = "*";

const std::string registrar = "xMsg_Registrar";

const std::string register_publisher = "registerPublisher";
const std::string register_subscriber = "registerSubscriber";
const int register_request_timeout = 3000;

const std::string remove_publisher = "removePublisherRegistration";
const std::string remove_subscriber = "removeSubscriberRegistration";
const std::string remove_all_registration = "removeAllRegistration";
const int remove_request_timeout = 3000;

const std::string find_publisher = "findPublisher";
const std::string find_subscriber = "findSubscriber";
const int find_request_timeout = 3000;

const std::string info = "INFO";
const std::string warning = "WARNING";
const std::string error = "ERROR";
const std::string done = "done";
const std::string data = "data";

const std::string no_result = "none";

const std::string bind = "bind";
const std::string connect = "connect";

const int default_port = 7771;
const int registrar_port = 8888;

} // end namespace constants


namespace mimetype {

const std::string single_sint32 = "binary/sint32";
const std::string single_sint64 = "binary/sint64";
const std::string single_sfixed32 = "binary/sfixed32";
const std::string single_sfixed64 = "binary/sfixed64";
const std::string single_float = "binary/float";
const std::string single_double = "binary/double";
const std::string single_string = "text/string";
const std::string bytes = "binary/bytes";

const std::string array_sint32 = "binary/array-sint32";
const std::string array_sint64 = "binary/array-sint64";
const std::string array_sfixed32 = "binary/array-sfixed32";
const std::string array_sfixed64 = "binary/array-sfixed32";
const std::string array_float = "binary/array-float";
const std::string array_double = "binary/array-double";
const std::string array_string = "binary/array-string";
const std::string array_bytes = "binary/array-bytes";

const std::string xmsg_data = "binary/native";
const std::string java_object = "binary/java";
const std::string cpp_object = "binary/cpp";
const std::string python_object = "binary/python";

} // end namespace mime

} // end namespace xmsg

#endif  // XMSG_CORE_CONSTANTS_H_
