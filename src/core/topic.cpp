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

#include "topic.h"
#include "constants.h"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

namespace {

const char SEPARATOR = ':';

}


namespace xmsg {

const std::string Topic::ANY = constants::any;


Topic Topic::build(const std::string& domain,
                   const std::string& subject,
                   const std::string& type) {
    if (domain == ANY) {
        throw std::invalid_argument("domain is not defined");
    }
    std::stringstream topic;
    topic << domain;
    if (subject != ANY) {
        topic << SEPARATOR << subject;
        if (type != ANY) {
            std::stringstream ss{type};
            std::string tst;
            while (std::getline(ss, tst, SEPARATOR)) {
                if (tst != ANY) {
                    topic << SEPARATOR << tst;
                } else {
                    break;
                }
            }
        }
    } else {
        if (type != ANY) {
            throw std::invalid_argument("subject is not defined");
        }
    }


    return { topic.str() };
}


std::string Topic::domain() const {
    auto firstSep = topic_.find(SEPARATOR);
    if (firstSep == std::string::npos) {
        return { topic_ };
    }
    return topic_.substr(0, firstSep);
}


std::string Topic::subject() const {
    auto firstSep = topic_.find(SEPARATOR);
    if (firstSep == std::string::npos) {
        return { ANY };
    }
    auto secondSep = topic_.find(SEPARATOR, firstSep + 1);
    if (secondSep == std::string::npos) {
        return topic_.substr(firstSep + 1);
    }
    return topic_.substr(firstSep + 1, secondSep - firstSep - 1);
}


std::string Topic::type() const {
    auto firstSep = topic_.find(SEPARATOR);
    if (firstSep == std::string::npos) {
        return { ANY };
    }
    auto secondSep = topic_.find(SEPARATOR, firstSep + 1);
    if (secondSep == std::string::npos) {
        return { ANY };
    }
    return topic_.substr(secondSep + 1);
}


bool Topic::is_parent(const Topic& other) const
{
    return !other.str().compare(0, topic_.size(), topic_);
}

} // end namespace xmsg
