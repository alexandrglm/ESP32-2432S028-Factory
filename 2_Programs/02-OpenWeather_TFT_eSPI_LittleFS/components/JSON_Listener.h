
#ifndef JSON_LISTENER_H
#define JSON_LISTENER_H


#pragma once

#include <Arduino.h>


class JsonListener {
  private:

  public:

    virtual ~JsonListener() {}

    // Member functions changed to a more logical order
    virtual void startDocument();

    virtual void endDocument();


    virtual void startObject();

    virtual void endObject();


    virtual void startArray();

    virtual void endArray();


    virtual void key(const char *key);

    virtual void value(const char *value);


    virtual void whitespace(char c);


    virtual void error( const char *message );

};

#endif // JSON_LISTENER_H
