/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2016 Tianjin KYLIN Information Technology Co., Ltd.
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

#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
#include "network-proxy.h"

/* With GSettings migration, UKUI is going to share proxy settings with
 * GNOME3 (in gsettings-desktop-schemas package).
 * GSettings schemas is http://git.gnome.org/browse/gsettings-desktop-schemas/tree/
 * schemas/org.gnome.system.proxy.gschema.xml.in.in
 * Sharing the same schema, we lose the locations feature.
 */

/*
 * FIXME use enum from gsettings-desktop-schema
 * http://git.gnome.org/browse/gsettings-desktop-schemas/tree/headers/gdesktop-enums.h#n26
 */
enum ProxyMode
{
    PROXYMODE_NONE,
    PROXYMODE_MANUAL,
    PROXYMODE_AUTO
};

enum {
    COL_NAME,
    COL_STYLE
};

typedef struct _NetworkProxy NetworkProxy;
struct _NetworkProxy {
    GtkBuilder * builder;
};
NetworkProxy networkproxy;

#define PROXY_SCHEMA              "org.gnome.system.proxy"
#define PROXY_MODE_KEY            "mode"
#define PROXY_AUTOCONFIG_URL_KEY  "autoconfig-url"
#define IGNORE_HOSTS_KEY          "ignore-hosts"

#define HTTP_PROXY_SCHEMA         "org.gnome.system.proxy.http"
#define HTTP_PROXY_HOST_KEY       "host"
#define HTTP_PROXY_PORT_KEY       "port"
#define HTTP_USE_AUTH_KEY         "use-authentication"
#define HTTP_AUTH_USER_KEY        "authentication-user"
#define HTTP_AUTH_PASSWD_KEY      "authentication-password"

#define HTTPS_PROXY_SCHEMA        "org.gnome.system.proxy.https"
#define SECURE_PROXY_HOST_KEY     "host"
#define SECURE_PROXY_PORT_KEY     "port"

#define FTP_PROXY_SCHEMA          "org.gnome.system.proxy.ftp"
#define FTP_PROXY_HOST_KEY        "host"
#define FTP_PROXY_PORT_KEY        "port"

#define SOCKS_PROXY_SCHEMA        "org.gnome.system.proxy.socks"
#define SOCKS_PROXY_HOST_KEY      "host"
#define SOCKS_PROXY_PORT_KEY      "port"

static GtkWidget *details_dialog = NULL;
static GSList *ignore_hosts = NULL;
static GtkTreeModel *model = NULL;

static GSettings *proxy_settings = NULL;
static GSettings *http_proxy_settings = NULL;
static GSettings *https_proxy_settings = NULL;
static GSettings *ftp_proxy_settings = NULL;
static GSettings *socks_proxy_settings = NULL;

static GtkTreeModel *
create_listmodel(void)
{
    GtkListStore *store;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    return GTK_TREE_MODEL(store);
}

static GtkTreeModel *
populate_listmodel(GtkListStore *store, GSList *list)
{
    GtkTreeIter iter;
    GSList *pointer;

    gtk_list_store_clear(store);

    pointer = list;
    while(pointer)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, (char *) pointer->data, -1);
        pointer = g_slist_next(pointer);
    }

    return GTK_TREE_MODEL(store);
}

static GtkWidget *
config_treeview(GtkTreeView *tree, GtkTreeModel *model)
{
    GtkCellRenderer *renderer;

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree),
                                                -1, "Hosts", renderer,
                                                "text", 0, NULL);

    gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);

    return GTK_WIDGET(tree);
}

static GtkWidget*
_gtk_builder_get_widget (GtkBuilder *builder, const gchar *name)
{
    return GTK_WIDGET (gtk_builder_get_object (builder, name));
}

static void
read_ignore_hosts_from_gsettings (void)
{
    gchar **array;
    ignore_hosts = NULL;
    gint i;
    array = g_settings_get_strv (proxy_settings, IGNORE_HOSTS_KEY);
    if (array != NULL)
    {
        for (i = 0; array[i]; i++)
        {
            ignore_hosts = g_slist_append (ignore_hosts, g_strdup (array[i]));
        }
    }
    g_strfreev (array);
}

static void
save_ignore_hosts_to_gsettings (void)
{
    GArray *array;
    GSList *l;
    array = g_array_new (TRUE, TRUE, sizeof (gchar *));
    for (l = ignore_hosts; l; l = l->next) 
    {
        array = g_array_append_val (array, l->data);
    }
    g_settings_set_strv (proxy_settings, IGNORE_HOSTS_KEY, (const gchar **) array->data);
    g_array_free (array, TRUE);
}

static void
cb_add_url (GtkButton *button, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER (data);
    gchar *new_url = NULL;

    new_url = g_strdup (gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object (builder, "entry_url"))));
    if (strlen (new_url) == 0)
        return;
    ignore_hosts = g_slist_append(ignore_hosts, new_url);
    populate_listmodel(GTK_LIST_STORE(model), ignore_hosts);
    gtk_entry_set_text(GTK_ENTRY (gtk_builder_get_object (builder,
                                 "entry_url")), "");

    save_ignore_hosts_to_gsettings ();
}

static void
cb_remove_url (GtkButton *button, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER (data);
    GtkTreeSelection *selection;
    GtkTreeIter       iter;

    selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (gtk_builder_get_object (builder, "treeview_ignore_host")));
    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gchar *url;
        GSList *pointer;

        gtk_tree_model_get (model, &iter, 0, &url, -1);

        pointer = ignore_hosts;
        while(pointer)
        {
            if(strcmp(url, (char *) pointer->data) == 0)
            {
                g_free (pointer->data);
                ignore_hosts = g_slist_delete_link(ignore_hosts, pointer);
                break;
            }
            pointer = g_slist_next(pointer);
        }

        g_free(url);
        populate_listmodel(GTK_LIST_STORE(model), ignore_hosts);

        save_ignore_hosts_to_gsettings ();
    }
}

static void network_proxy_details_respone(GtkDialog *dialog, gint response_id, gpointer user_data){
    gtk_widget_destroy(GTK_WIDGET(dialog));
    details_dialog = NULL;
}

static void
cb_use_auth_toggled (GtkToggleButton *toggle,
             GtkWidget *table)
{
    gtk_widget_set_sensitive (table, gtk_toggle_button_get_active (toggle));
}

static void
cb_http_details_button_clicked (GtkWidget *button,
                    GtkWidget *parent)
{
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *widget;
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, UIDIR "/network.ui", &error);
	if(error != NULL){
		g_warning("Load network.ui false, error=%s\n",error->message);
		g_error_free(error);
		g_object_unref(builder);
		return;
	}
	
    details_dialog = widget = _gtk_builder_get_widget (builder,
                               "network_proxy_details_dialog");
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW(widget), TRUE);
    //gtk_window_set_transient_for (GTK_WINDOW (widget), GTK_WINDOW (parent));
	//if(details_dialog != NULL){
	//	gtk_window_present(GTK_WINDOW(details_dialog));
	//	return;
	//}
    g_signal_connect (gtk_builder_get_object (builder, "use_auth_checkbutton"),
              "toggled",
              G_CALLBACK (cb_use_auth_toggled),
              _gtk_builder_get_widget (builder, "auth_table"));

    g_settings_bind (http_proxy_settings, HTTP_USE_AUTH_KEY,
            gtk_builder_get_object (builder, "use_auth_checkbutton"), "active",
            G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (http_proxy_settings, HTTP_AUTH_USER_KEY,
            gtk_builder_get_object (builder, "username_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (http_proxy_settings, HTTP_AUTH_PASSWD_KEY,
            gtk_builder_get_object (builder, "password_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);

    g_signal_connect(details_dialog, "response", G_CALLBACK(network_proxy_details_respone), NULL);
    gtk_widget_show_all (widget);
}

static void
proxy_mode_gsettings_changed (GSettings *settings,
                   gchar *key,
                   GtkBuilder *builder)
{
    int mode;
    mode = g_settings_get_enum (settings, PROXY_MODE_KEY);
    if (mode == PROXYMODE_NONE)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gtk_builder_get_object(builder, "none_radiobutton")), TRUE);
    else if (mode == PROXYMODE_AUTO)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gtk_builder_get_object(builder, "auto_radiobutton")), TRUE);
    else if (mode == PROXYMODE_MANUAL)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gtk_builder_get_object(builder, "manual_radiobutton")), TRUE);
    gtk_widget_set_sensitive (_gtk_builder_get_widget (builder, "manual_box"),
                  mode == PROXYMODE_MANUAL);
    gtk_widget_set_sensitive (_gtk_builder_get_widget (builder, "auto_box"),
                  mode == PROXYMODE_AUTO);
}

static void
proxy_mode_radiobutton_clicked_cb (GtkWidget *widget,
                   GtkBuilder *builder)
{
    GSList *mode_group;
    int mode;
    int old_mode;

    if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
        return;

    mode_group = g_slist_copy (gtk_radio_button_get_group
        ((GtkRadioButton *)GTK_WIDGET(gtk_builder_get_object (builder, "none_radiobutton"))));
    mode_group = g_slist_reverse (mode_group);
    mode = g_slist_index (mode_group, widget);
    g_slist_free (mode_group);

    old_mode = g_settings_get_enum (proxy_settings, PROXY_MODE_KEY);

    if (mode == old_mode)
        return;

    g_settings_set_enum (proxy_settings, PROXY_MODE_KEY, mode);
}

static void
connect_sensitivity_signals (GtkBuilder *builder, GSList *mode_group)
{
    for (; mode_group != NULL; mode_group = mode_group->next)
    {
        g_signal_connect (G_OBJECT (mode_group->data), "clicked",
                  G_CALLBACK(proxy_mode_radiobutton_clicked_cb),
                  builder);
    }
}

static void
cb_ignore_hosts_gsettings_changed (GSettings *settings, gchar *key, gpointer user_data)
{
    g_slist_foreach (ignore_hosts, (GFunc) g_free, NULL);
    g_slist_free (ignore_hosts);

    read_ignore_hosts_from_gsettings ();

    populate_listmodel (GTK_LIST_STORE (model), ignore_hosts);
}

static void
setup_dialog (GtkBuilder *builder)
{
    GSList *mode_group;
    GtkListStore *store;

    g_signal_connect (proxy_settings, "changed::" IGNORE_HOSTS_KEY,
            G_CALLBACK (cb_ignore_hosts_gsettings_changed), NULL);

    /* Mode */
    proxy_mode_gsettings_changed (proxy_settings, PROXY_MODE_KEY, builder);
    mode_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "none_radiobutton")));
    connect_sensitivity_signals (builder, mode_group);
    g_signal_connect (proxy_settings, "changed::" PROXY_MODE_KEY,
                      G_CALLBACK (proxy_mode_gsettings_changed), builder);

    /* Http */
    g_settings_bind (http_proxy_settings, HTTP_PROXY_PORT_KEY,
            gtk_builder_get_object (builder, "http_port_spinbutton"), "value",
            G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (http_proxy_settings, HTTP_PROXY_HOST_KEY,
            gtk_builder_get_object (builder, "http_host_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);
    g_signal_connect (gtk_builder_get_object (builder, "details_button1"),
              "clicked",
              G_CALLBACK (cb_http_details_button_clicked),
              _gtk_builder_get_widget (builder, "dialog-vbox1"));

    /* Secure */
    g_settings_bind (https_proxy_settings, SECURE_PROXY_PORT_KEY,
            gtk_builder_get_object (builder, "secure_port_spinbutton"), "value",
            G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (https_proxy_settings, SECURE_PROXY_HOST_KEY,
            gtk_builder_get_object (builder, "secure_host_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);

    /* Ftp */
    g_settings_bind (ftp_proxy_settings, FTP_PROXY_PORT_KEY,
            gtk_builder_get_object (builder, "ftp_port_spinbutton"), "value",
            G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (ftp_proxy_settings, FTP_PROXY_HOST_KEY,
            gtk_builder_get_object (builder, "ftp_host_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);

    /* Socks */
    g_settings_bind (socks_proxy_settings, SOCKS_PROXY_PORT_KEY,
            gtk_builder_get_object (builder, "socks_port_spinbutton"), "value",
            G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (socks_proxy_settings, SOCKS_PROXY_HOST_KEY,
            gtk_builder_get_object (builder, "socks_host_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);

    /* Autoconfiguration */
    g_settings_bind (proxy_settings, PROXY_AUTOCONFIG_URL_KEY,
            gtk_builder_get_object (builder, "autoconfig_entry"), "text",
            G_SETTINGS_BIND_DEFAULT);

    //g_signal_connect (gtk_builder_get_object (builder, "network_dialog"),
    //		"response", G_CALLBACK (cb_dialog_response), NULL);

    read_ignore_hosts_from_gsettings ();

    model = create_listmodel();
    populate_listmodel(GTK_LIST_STORE(model), ignore_hosts);
    config_treeview(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview_ignore_host")), model);

    g_signal_connect (gtk_builder_get_object (builder, "button_add_url"),
              "clicked", G_CALLBACK (cb_add_url), builder);
    g_signal_connect (gtk_builder_get_object (builder, "entry_url"),
              "activate", G_CALLBACK (cb_add_url), builder);
    g_signal_connect (gtk_builder_get_object (builder, "button_remove_url"),
              "clicked", G_CALLBACK (cb_remove_url), builder);
}

void add_network_app(GtkBuilder * builder)
{
    g_debug("add_network_app");
    networkproxy.builder = builder;
    proxy_settings = g_settings_new(PROXY_SCHEMA);
    http_proxy_settings = g_settings_new(HTTP_PROXY_SCHEMA);
    https_proxy_settings = g_settings_new(HTTPS_PROXY_SCHEMA);
    ftp_proxy_settings = g_settings_new(FTP_PROXY_SCHEMA);
    socks_proxy_settings = g_settings_new(SOCKS_PROXY_SCHEMA);
    setup_dialog(builder);
}

void destory_network_app()
{
    if (ignore_hosts) 
    {
        g_slist_foreach (ignore_hosts, (GFunc) g_free, NULL);
        g_slist_free (ignore_hosts);
    }
    g_object_unref (proxy_settings);
    g_object_unref (http_proxy_settings);
    g_object_unref (https_proxy_settings);
    g_object_unref (ftp_proxy_settings);
    g_object_unref (socks_proxy_settings);
}


