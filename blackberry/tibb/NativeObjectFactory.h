/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef NATIVEOBJECTFACTORY_H_
#define NATIVEOBJECTFACTORY_H_

#include "TiEventContainerFactory.h"

class NativeObject;
class NativeContainerObject;
class TiCascadesApp;

/*
 * NativeObjectFactory
 *
 * Creates native control objects
 *
 */

class NativeObjectFactory
{
public:
    NativeObjectFactory(TiCascadesApp* cascadesApp);
    virtual ~NativeObjectFactory();
    NativeObject* createNativeObject(int type);
    NativeObject* getRootContainer() const;
    void setRootContainer(NativeObject* container);
    void setEventContainerFactory(TiEventContainerFactory* eventContainerFactory);
    TiEventContainerFactory* getEventContainerFactory() const;

private:
    TiCascadesApp* cascadesApp_;
    NativeObject* rootContainer_;
    TiEventContainerFactory* eventContainerFactory_;
};

#endif /* NATIVEOBJECTFACTORY_H_ */
