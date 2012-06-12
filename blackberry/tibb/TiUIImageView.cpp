/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#include "TiUIImageView.h"

TiUIImageView::TiUIImageView(NativeObjectFactory* nativeObjectFactory)
    : TiUIBase(nativeObjectFactory, "")
{
}

TiUIImageView::~TiUIImageView()
{
}

TiUIBase* TiUIImageView::createImageView(NativeObjectFactory* nativeObjectFactory)
{
    TiUIImageView* obj = new TiUIImageView(nativeObjectFactory);
    obj->initializeTiObject(NULL);
    return obj;
}

void TiUIImageView::initializeTiObject(TiObject* parentContext)
{
    if (!isInitialized())
    {
        TiUIBase::initializeTiObject(parentContext);
        NativeObject* obj = getNativeObjectFactory()->createNativeObject(N_TYPE_IMAGEVIEW);
        setNativeObject(obj);
        obj->release();
    }
}
