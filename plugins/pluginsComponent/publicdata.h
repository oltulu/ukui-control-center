/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef PUBLICDATA_H
#define PUBLICDATA_H


#include<QList>
#include <QObject>

class PublicData
{

public:
    explicit PublicData();
    ~PublicData();

    QList<QStringList> subfuncList;
    //列表中的字符经过国际化，但是qt资源文件使用中文文件名导致pbuilder编译报错，not found
    //特别增加英文列表供左侧边栏的logo调用
    QList<QStringList> en_subfuncList;

};

#endif // PUBLICDATA_H