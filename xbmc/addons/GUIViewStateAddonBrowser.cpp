/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "GUIViewStateAddonBrowser.h"
#include "FileItem.h"
#include "guilib/Key.h"
#include "ViewState.h"
#include "settings/Settings.h"
#include "addons/Addon.h"
#include "addons/AddonManager.h"
#include "AddonDatabase.h"

using namespace XFILE;
using namespace ADDON;

CGUIViewStateAddonBrowser::CGUIViewStateAddonBrowser(const CFileItemList& items) : CGUIViewState(items)
{
  AddSortMethod(SORT_METHOD_LABEL_IGNORE_FOLDERS, 551, LABEL_MASKS("%L", "%I", "%L", ""));  // Filename, Size | Foldername, empty
  AddSortMethod(SORT_METHOD_DATE, 552, LABEL_MASKS("%L", "%J", "%L", "%J"));  // Filename, Date | Foldername, Date
  SetSortMethod(SORT_METHOD_LABEL_IGNORE_FOLDERS);

  SetViewAsControl(DEFAULT_VIEW_AUTO);

  SetSortOrder(SORT_ORDER_ASC);
  LoadViewState(items.m_strPath, WINDOW_ADDON_BROWSER);
}

void CGUIViewStateAddonBrowser::SaveViewState()
{
  SaveViewToDb(m_items.m_strPath, WINDOW_ADDON_BROWSER);
}

CStdString CGUIViewStateAddonBrowser::GetExtensions()
{
  return "";
}

VECSOURCES& CGUIViewStateAddonBrowser::GetSources()
{
  m_sources.clear();

  // we always have some enabled addons
  {
    CMediaSource share;
    share.strPath = "addons://enabled/";
    share.m_iDriveType = CMediaSource::SOURCE_TYPE_LOCAL;
    share.strName = g_localizeStrings.Get(24062);
    m_sources.push_back(share);
  }
  CAddonDatabase db;
  if (db.Open() && db.HasDisabledAddons())
  {
    CMediaSource share;
    share.strPath = "addons://disabled/";
    share.m_iDriveType = CMediaSource::SOURCE_TYPE_LOCAL;
    share.strName = g_localizeStrings.Get(24039);
    m_sources.push_back(share);
  }
  if (CAddonMgr::Get().HasOutdatedAddons())
  {
    CMediaSource share;
    share.strPath = "addons://outdated/";
    share.m_iDriveType = CMediaSource::SOURCE_TYPE_LOCAL;
    share.strName = g_localizeStrings.Get(24043);
    m_sources.push_back(share);
  }
  if (CAddonMgr::Get().HasAddons(ADDON_REPOSITORY,true))
  {
    CMediaSource share;
    share.strPath = "addons://repos/";
    share.m_iDriveType = CMediaSource::SOURCE_TYPE_LOCAL;
    share.strName = g_localizeStrings.Get(24033);
    m_sources.push_back(share);
  }
  // add "install from zip"
  {
    CMediaSource share;
    share.strPath = "addons://install/";
    share.m_iDriveType = CMediaSource::SOURCE_TYPE_LOCAL;
    share.strName = g_localizeStrings.Get(24041);
    m_sources.push_back(share);
  }

  return CGUIViewState::GetSources();
}

