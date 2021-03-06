/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#include "TiPropertyMapObject.h"

TiPropertyMapObject::TiPropertyMapObject(const char* name)
    : TiObject(name)
{
}

TiPropertyMapObject::~TiPropertyMapObject()
{
}

TiPropertyMapObject* TiPropertyMapObject::addProperty(TiObject* parent, const char* name, int propertyNumber,
        MODIFY_VALUE_CALLBACK cb, void* context)
{
    TiPropertyMapObject* object = new TiPropertyMapObject(name);
    object->propertyNumber_ = propertyNumber;
    object->callback_ = cb;
    object->context_ = context;
    parent->addMember(object);
    return object;
}

VALUE_MODIFY TiPropertyMapObject::onValueChange(Handle<Value> oldValue, Handle<Value> newValue)
{
    HandleScope handleScope;
    VALUE_MODIFY modify = TiObject::onValueChange(oldValue, newValue);
    Handle<String> stringValue;
    if (modify != VALUE_MODIFY_ALLOW)
    {
        return modify;
    }
    forceSetValue(newValue);
    modify = (callback_)(propertyNumber_, this, context_);
    if (modify != VALUE_MODIFY_ALLOW)
    {
        forceSetValue(oldValue);
    }
    return modify;
}
