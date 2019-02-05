/*******************************************************************************
* SCHNApps                                                                     *
* Copyright (C) 2015, IGG Group, ICube, University of Strasbourg, France       *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#ifndef SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_DIALOG_COMPUTE_FEATURE_LINES_H_
#define SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_DIALOG_COMPUTE_FEATURE_LINES_H_

#include <schnapps/plugins/surface_feature_lines/dll.h>

#include <schnapps/core/types.h>

#include <ui_dialog_compute_feature_lines.h>

namespace cgogn { enum Orbit: numerics::uint32; }

namespace schnapps
{

namespace plugin_cmap_provider
{
class Plugin_CMapProvider;
class CMap2Handler;
}

class SCHNApps;
class Object;

namespace plugin_surface_feature_lines
{

class Plugin_SurfaceFeatureLines;
using CMap2Handler = plugin_cmap_provider::CMap2Handler;

class SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_API ComputeFeatureLines_Dialog : public QDialog, public Ui::ComputeFeatureLines_Dialog
{
	Q_OBJECT

public:

	ComputeFeatureLines_Dialog(SCHNApps* s, Plugin_SurfaceFeatureLines* p);
	~ComputeFeatureLines_Dialog() override;

private:

	SCHNApps* schnapps_;
	Plugin_SurfaceFeatureLines* plugin_;

	plugin_cmap_provider::Plugin_CMapProvider* plugin_cmap_provider_;

	CMap2Handler* selected_map_;

	bool updating_ui_;

	QString setting_auto_load_position_attribute_;
	QString setting_auto_load_Kmax_attribute_;
	QString setting_auto_load_kmax_attribute_;
	QString setting_auto_load_Kmin_attribute_;
	QString setting_auto_load_kmin_attribute_;

private slots:

	// slots called from UI signals
	void selected_map_changed();
	void compute_feature_lines();

	// slots called from SCHNApps signals
	void object_added(Object* o);
	void object_removed(Object* o);

	// slots called from CMap2Handler signals
	void selected_map_attribute_added(cgogn::Orbit orbit, const QString& attribute_name);
	void selected_map_attribute_removed(cgogn::Orbit orbit, const QString& attribute_name);

private:

	void refresh_ui();

	void map_added(CMap2Handler* mh);
	void map_removed(CMap2Handler* mh);
};

} // namespace plugin_surface_feature_lines

} // namespace schnapps

#endif // SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_DIALOG_COMPUTE_FEATURE_LINES_H_
