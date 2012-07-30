/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#include "NativeListViewObject.h"

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/DataModel>
#include <bb/cascades/ListView>
#include <bb/cascades/QListDataModel>
#include "TiEventContainerFactory.h"

NativeListViewObject::NativeListViewObject()
{
    listView_ = NULL;
    top_ = 0;
    left_ = 0;
}

NativeListViewObject::~NativeListViewObject()
{
}

NativeListViewObject* NativeListViewObject::createListView()
{
    return new NativeListViewObject;
}

int NativeListViewObject::initialize(TiEventContainerFactory* containerFactory)
{
    listView_ = bb::cascades::ListView::create();
    setControl(listView_);
    TiEventContainer* eventClicked = containerFactory->createEventContainer();
    eventClicked->setDataProperty("type", tetCLICK);
    events_.insert(tetCLICK, new EventPair(eventClicked, new ListViewEventHandler(eventClicked, this)));
    QObject::connect(listView_, SIGNAL(selectionChanged(QVariantList, bool)), events_[tetCLICK]->handler, SLOT(selectionChanged(QVariantList, bool)));
    return NATIVE_ERROR_OK;
}

int NativeListViewObject::setData(TiObject* obj)
{
    QVector<QPair<QString, QString> > dictionary;
    int error = NativeControlObject::getDictionaryData(obj, dictionary);
    if (!N_SUCCEEDED(error))
    {
        return error;
    }
    QList<QString> dataList;
    for (int i = 0; i < dictionary.size(); ++i)
    {
        //TODO define const var, for title string
        if (dictionary[i].first == "title")
        {
            dataList.append(dictionary[i].second);
        }
    }
    listView_->setDataModel(new bb::cascades::QListDataModel<QString>(dataList));
    return NATIVE_ERROR_OK;
}

int NativeListViewObject::getObjectType() const
{
    return N_TYPE_LIST_VIEW;
}

NAHANDLE NativeListViewObject::getNativeHandle() const
{
    return listView_;
}

QString NativeListViewObject::getListViewElementFromIndex(QVariantList var)
{
    bb::cascades::DataModel* dataM = listView_->dataModel();
    QVariant tmp = dataM->data(var);
    QString str = tmp.toString();
    return str;
}

int NativeListViewObject::scrollToIndex(int index)
{
    QVariant variant(index);
    QVariantList scroll;
    scroll.append(variant);
    listView_->scrollToItem(scroll);
    return NATIVE_ERROR_OK;
}
