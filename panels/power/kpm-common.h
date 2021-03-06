/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2005-2007 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __GPMCOMMON_H
#define __GPMCOMMON_H

#include <glib.h>

#include <unistd.h>

G_BEGIN_DECLS

#define LOGIND_RUNNING() (access("/run/systemd/seats/", F_OK) >= 0)

#define	GPM_DBUS_SERVICE		"org.ukui.PowerManager"
#define	GPM_DBUS_INTERFACE		"org.ukui.PowerManager"
#define	GPM_DBUS_INTERFACE_BACKLIGHT	"org.ukui.PowerManager.Backlight"
#define	GPM_DBUS_PATH			"/org/ukui/PowerManager"
#define	GPM_DBUS_PATH_BACKLIGHT		"/org/ukui/PowerManager/Backlight"
#define GPM_DBUS_PATH_KBD_BACKLIGHT    "/org/ukui/PowerManager/KbdBacklight"

/* common descriptions of this program */
#define GPM_NAME 			_("Power Manager")
#define GPM_DESCRIPTION 		_("Power Manager for the UKUI desktop")

/* schema location */
#define KPM_SETTINGS_SCHEMA				"org.ukui.power-manager"

/* actions */
#define GPM_SETTINGS_ACTION_CRITICAL_UPS		"action-critical-ups"
#define KPM_SETTINGS_ACTION_CRITICAL_BATT		"action-critical-battery"
#define GPM_SETTINGS_ACTION_LOW_UPS			"action-low-ups"
#define GPM_SETTINGS_ACTION_SLEEP_TYPE_AC		"action-sleep-type-ac"
#define GPM_SETTINGS_ACTION_SLEEP_TYPE_BATT		"action-sleep-type-battery"
#define GPM_SETTINGS_SLEEP_WHEN_CLOSED			"event-when-closed-battery"

/* backlight stuff */
#define GPM_SETTINGS_BACKLIGHT_ENABLE			"backlight-enable"
#define KPM_SETTINGS_BACKLIGHT_BATTERY_REDUCE		"backlight-battery-reduce"
#define GPM_SETTINGS_DPMS_METHOD_AC			"dpms-method-ac"
#define GPM_SETTINGS_DPMS_METHOD_BATT			"dpms-method-battery"
#define GPM_SETTINGS_IDLE_BRIGHTNESS			"idle-brightness"
#define KPM_SETTINGS_IDLE_DIM_AC			"idle-dim-ac"
#define KPM_SETTINGS_IDLE_DIM_BATT			"idle-dim-battery"
#define GPM_SETTINGS_IDLE_DIM_TIME			"idle-dim-time"
#define KPM_SETTINGS_BRIGHTNESS_AC			"brightness-ac"
#define GPM_SETTINGS_BRIGHTNESS_DIM_BATT		"brightness-dim-battery"

/* keyboard backlight */
#define GPM_SETTINGS_KBD_BACKLIGHT_BATT_REDUCE     "kbd-backlight-battery-reduce"
#define GPM_SETTINGS_KBD_BRIGHTNESS_ON_AC      "kbd-brightness-on-ac"
#define GPM_SETTINGS_KBD_BRIGHTNESS_DIM_BY_ON_BATT      "kbd-brightness-dim-by-on-battery"
#define GPM_SETTINGS_KBD_BRIGHTNESS_DIM_BY_ON_IDLE "kbd-brightness-dim-by-on-idle"

/* buttons */
#define KPM_SETTINGS_BUTTON_LID_AC			"button-lid-ac"
#define KPM_SETTINGS_BUTTON_LID_BATT			"button-lid-battery"
#define KPM_SETTINGS_BUTTON_SUSPEND			"button-suspend"
#define KPM_SETTINGS_BUTTON_HIBERNATE			"button-hibernate"
#define KPM_SETTINGS_BUTTON_POWER			"button-power"

/* general */
#define GPM_SETTINGS_USE_TIME_POLICY			"use-time-for-policy"
#define GPM_SETTINGS_NETWORKMANAGER_SLEEP		"network-sleep"
#define GPM_SETTINGS_IDLE_CHECK_CPU			"check-type-cpu"

/* lock */
#define GPM_SETTINGS_LOCK_USE_SCREENSAVER		"lock-use-screensaver"
#define GPM_SETTINGS_LOCK_ON_BLANK_SCREEN		"lock-blank-screen"
#define GPM_SETTINGS_LOCK_ON_SUSPEND			"lock-suspend"
#define GPM_SETTINGS_LOCK_ON_HIBERNATE			"lock-hibernate"
#define GPM_SETTINGS_LOCK_KEYRING_SUSPEND		"lock-keyring-suspend"
#define GPM_SETTINGS_LOCK_KEYRING_HIBERNATE		"lock-keyring-hibernate"

/* disks */
#define KPM_SETTINGS_SPINDOWN_ENABLE_AC			"spindown-enable-ac"
#define KPM_SETTINGS_SPINDOWN_ENABLE_BATT		"spindown-enable-battery"
#define GPM_SETTINGS_SPINDOWN_TIMEOUT_AC		"spindown-timeout-ac"
#define GPM_SETTINGS_SPINDOWN_TIMEOUT_BATT		"spindown-timeout-battery"

/* notify */
#define GPM_SETTINGS_NOTIFY_PERHAPS_RECALL		"notify-perhaps-recall"
#define GPM_SETTINGS_NOTIFY_LOW_CAPACITY		"notify-low-capacity"
#define GPM_SETTINGS_NOTIFY_DISCHARGING			"notify-discharging"
#define GPM_SETTINGS_NOTIFY_FULLY_CHARGED		"notify-fully-charged"
#define GPM_SETTINGS_NOTIFY_SLEEP_FAILED		"notify-sleep-failed"
#define GPM_SETTINGS_NOTIFY_SLEEP_FAILED_URI		"notify-sleep-failed-uri"
#define GPM_SETTINGS_NOTIFY_LOW_POWER			"notify-low-power"

/* thresholds */
#define GPM_SETTINGS_PERCENTAGE_LOW			"percentage-low"
#define GPM_SETTINGS_PERCENTAGE_CRITICAL		"percentage-critical"
#define GPM_SETTINGS_PERCENTAGE_ACTION			"percentage-action"
#define GPM_SETTINGS_TIME_LOW				"time-low"
#define GPM_SETTINGS_TIME_CRITICAL			"time-critical"
#define GPM_SETTINGS_TIME_ACTION			"time-action"

/* timeout */
#define KPM_SETTINGS_SLEEP_COMPUTER_AC			"sleep-computer-ac"
#define KPM_SETTINGS_SLEEP_COMPUTER_BATT		"sleep-computer-battery"
#define KPM_SETTINGS_SLEEP_COMPUTER_UPS			"sleep-computer-ups"
#define KPM_SETTINGS_SLEEP_DISPLAY_AC			"sleep-display-ac"
#define KPM_SETTINGS_SLEEP_DISPLAY_BATT			"sleep-display-battery"
#define KPM_SETTINGS_SLEEP_DISPLAY_UPS			"sleep-display-ups"

/* ui */
#define KPM_SETTINGS_ICON_POLICY			"icon-policy"
#define GPM_SETTINGS_ENABLE_SOUND			"enable-sound"
#define GPM_SETTINGS_SHOW_ACTIONS			"show-actions"

/* statistics */
#define GPM_SETTINGS_INFO_HISTORY_TIME			"info-history-time"
#define GPM_SETTINGS_INFO_HISTORY_TYPE			"info-history-type"
#define GPM_SETTINGS_INFO_HISTORY_GRAPH_SMOOTH		"info-history-graph-smooth"
#define GPM_SETTINGS_INFO_HISTORY_GRAPH_POINTS		"info-history-graph-points"
#define GPM_SETTINGS_INFO_STATS_TYPE			"info-stats-type"
#define GPM_SETTINGS_INFO_STATS_GRAPH_SMOOTH		"info-stats-graph-smooth"
#define GPM_SETTINGS_INFO_STATS_GRAPH_POINTS		"info-stats-graph-points"
#define GPM_SETTINGS_INFO_PAGE_NUMBER			"info-page-number"
#define GPM_SETTINGS_INFO_LAST_DEVICE			"info-last-device"

/* ukui-screensaver */
#define GS_SETTINGS_SCHEMA				"org.ukui.screensaver"
#define GS_SETTINGS_PREF_LOCK_ENABLED			"lock-enabled"

typedef enum {
	KPM_ICON_POLICY_ALWAYS,
	KPM_ICON_POLICY_PRESENT,
	KPM_ICON_POLICY_CHARGE,
	KPM_ICON_POLICY_LOW,
	KPM_ICON_POLICY_CRITICAL,
	KPM_ICON_POLICY_NEVER
} KpmIconPolicy;

typedef enum {
	KPM_ACTION_POLICY_BLANK,
	KPM_ACTION_POLICY_SUSPEND,
	KPM_ACTION_POLICY_SHUTDOWN,
	KPM_ACTION_POLICY_HIBERNATE,
	KPM_ACTION_POLICY_INTERACTIVE,
	KPM_ACTION_POLICY_NOTHING
} KpmActionPolicy;

gchar		*kpm_get_timestring				(guint		 time);
guint        gpm_discrete_from_percent          (guint       percentage,
                                guint        levels);
guint        gpm_discrete_to_percent            (guint       discrete,
                                guint        levels);
void 		 gpm_help_display				(const gchar	*link_id);
#ifdef EGG_TEST
void		 gpm_common_test				(gpointer	 data);
#endif

G_END_DECLS

#endif	/* __GPMCOMMON_H */
