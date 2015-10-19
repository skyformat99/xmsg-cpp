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

#ifndef XMSG_CORE_TOPIC_H_
#define XMSG_CORE_TOPIC_H_

#include <string>
#include <utility>

namespace xmsg {

/**
 * The main identification for xMsg pub/sub communications.
 * xMsg is a <b>topic-based</b> system, and messages are published to given
 * "topics", or named channels, defined by <i>publishers</i>.
 * <i>Subscribers</i> can received the messages published to the "topics" to
 * which they are interested, by subscribing to them.
 * <p>
 * In xMsg, a topic is composed of three parts: a <b>domain</b>, a
 * <b>subject</b> and a <b>type</b>. Each part is separated by a semicolon
 * character. The subject and the type can be omitted. Thus, the following
 * topics are all valid:
 * <ul>
 * <li>{@code "domain"}
 * <li>{@code "domain:subject"}
 * <li>{@code "domain:subject:type"}
 * </ul>
 * The {@link #build} factory methods help to create a proper topic with the
 * right format.
 * <p>
 * When an {@link xMsg} actor is subscribed to a given topic, it will only
 * receive messages published to that topic. To filter topics, the three  parts
 * form a hierarchy, and all topics with the same prefix will be accepted.
 * <p>
 * In other words, a subscriber listening for an specific "domain" will receive
 * all messages whose topic starts with that domain, no matter the subject and
 * the type. For example, if the subscription topic is {@code "A"}, then all the
 * messages with the following topics will be received:
 * <ul>
 * <li>{@code "A"}
 * <li>{@code "A:B"}
 * <li>{@code "A:C"}
 * <li>{@code "A:B:1"}
 * <li>{@code "A:C:1"}
 * <li>{@code "A:C:2"}
 * <li>etc...
 * </ul>
 * More specific subscriptions will not receive messages that match only the
 * parent parts of the topic.
 * Thus, subscription to {@code "A:B"} will accept {@code "A:B"}, {@code
 * "A:B:1"}, {@code "A:B:2"}, etc, but will reject {@code "A"} or {@code "A:C"}.
 * Similarly, a subscription to {@code "A:B:1"} will only accept that exact
 * topic, rejecting {@code "A:B:2"}, {@code "A:C"}, {@code "A"}, etc.
 */
class Topic final
{
public:
    /**
     * Builds a new topic with only a domain part.
     *
     * @param domain the domain of the topic
     */
    static Topic build(const std::string& domain)
    {
        return build(domain, ANY, ANY);
    }

    /**
     * Builds a new topic with only domain and subject parts.
     *
     * @param domain the domain of the topic
     * @param subject the subject of the topic
     */
    static Topic build(const std::string& domain,
                       const std::string& subject)
    {
        return build(domain, subject, ANY);
    }

    /**
     * Builds a new full topic with domain, subject and type.
     *
     * @param domain the domain of the topic
     * @param subject the subject of the topic
     * @param type the type of the subject
     */
    static Topic build(const std::string& domain,
                       const std::string& subject,
                       const std::string& type);

    /**
     * Use the given string as an xMsg topic.
     * No validation is done to the string.
     * The caller must be sure it is a valid topic.
     * This factory method is provided for speed purposes.
     * It should be used with caution.
     *
     * @param topic a valid xMsg topic string
     */
    static Topic raw(const std::string& topic)
    {
        return { topic };
    }

    /**
     * Use the given string as an xMsg topic.
     * No validation is done to the string.
     * The caller must be sure it is a valid topic.
     * This factory method is provided for speed purposes.
     * It should be used with caution.
     *
     * @param topic a valid xMsg topic string
     */
    static Topic raw(std::string&& topic)
    {
        return { std::move(topic) };
    }


    static const std::string ANY;

public:
    /**
     * Returns the type part of the topic.
     */
    std::string domain() const;

    /**
     * Returns the type part of the topic. If the topic has no subject, then
     * {@code "*"} is returned.
     */
    std::string subject() const;

    /**
     * Returns the type part of the topic. If the topic has no type, then
     * {@code "*"} is returned.
     */
    std::string type() const;

    /**
     * Returns true if this topic is a parent of the given topic.
     * A parent topic is a prefix of other topic, or they are the same.
     * Examples:
     * <ul>
     * <li>{@code "A"} is a parent of {@code "A:B"} and {@code "A:C:1"}
     * <li>{@code "A"} is NOT parent of {@code "W:B"} nor {@code "Z"}
     * <li>{@code "A:C"} is a parent of {@code "A:C:1"} and {@code "A:C"}
     * <li>{@code "A:C"} is NOT a parent of {@code "A:B"}
     * </ul>
     * A subscription to a parent topic will accept any children topic.
     * See the class documentation for more details about filtering messages by
     * topic.
     *
     * @param other the topic to match as a children
     * @return true if this topic is a parent of the other
     */
    bool is_parent(const Topic& other) const;

    /**
     * Returns the string representation of this topic.
     */
    const std::string& str() const
    {
        return topic_;
    };

private:
    /**
     * Construct a topic from the given string.
     * The string must be a valid topic.
     */
    template <
        typename T,
        typename = std::enable_if_t<
            !std::is_same<Topic, std::decay_t<T>>::value
        >
    >
    Topic(T&& topic)
        : topic_{std::forward<T>(topic)} { }

private:
    std::string topic_;
};


inline bool operator==(const Topic& lhs, const Topic& rhs)
{
    return lhs.str() == rhs.str();
}


inline bool operator!=(const Topic& lhs, const Topic& rhs)
{
    return !(lhs == rhs);
}

} // end namespace xmsg

#endif // XMSG_CORE_TOPIC_H_
