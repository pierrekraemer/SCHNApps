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

#include <schnapps/plugins/surface_feature_lines/dialog_compute_feature_lines.h>
#include <schnapps/plugins/surface_feature_lines/surface_feature_lines.h>

#include <schnapps/plugins/cmap_provider/cmap_provider.h>

#include <schnapps/core/schnapps.h>

namespace schnapps
{

namespace plugin_surface_feature_lines
{

ComputeFeatureLines_Dialog::ComputeFeatureLines_Dialog(SCHNApps* s, Plugin_SurfaceFeatureLines* p) :
	schnapps_(s),
	plugin_(p),
	selected_map_(nullptr),
	updating_ui_(false)
{
	setupUi(this);

	if (plugin_->setting("Auto load position attribute").isValid())
		setting_auto_load_position_attribute_ = plugin_->setting("Auto load position attribute").toString();
	else
		setting_auto_load_position_attribute_ = plugin_->add_setting("Auto load position attribute", "position").toString();

	if (plugin_->setting("Auto load Kmax attribute").isValid())
		setting_auto_load_Kmax_attribute_ = plugin_->setting("Auto load Kmax attribute").toString();
	else
		setting_auto_load_Kmax_attribute_ = plugin_->add_setting("Auto load Kmax attribute", "Kmax").toString();

	if (plugin_->setting("Auto load kmax attribute").isValid())
		setting_auto_load_kmax_attribute_ = plugin_->setting("Auto load kmax attribute").toString();
	else
		setting_auto_load_kmax_attribute_ = plugin_->add_setting("Auto load kmax attribute", "kmax").toString();

	if (plugin_->setting("Auto load Kmin attribute").isValid())
		setting_auto_load_Kmin_attribute_ = plugin_->setting("Auto load Kmin attribute").toString();
	else
		setting_auto_load_Kmin_attribute_ = plugin_->add_setting("Auto load Kmin attribute", "Kmin").toString();

	if (plugin_->setting("Auto load kmin attribute").isValid())
		setting_auto_load_kmin_attribute_ = plugin_->setting("Auto load kmin attribute").toString();
	else
		setting_auto_load_kmin_attribute_ = plugin_->add_setting("Auto load kmin attribute", "kmin").toString();

	connect(list_maps, SIGNAL(itemSelectionChanged()), this, SLOT(selected_map_changed()));

	connect(this, SIGNAL(accepted()), this, SLOT(compute_feature_lines()));
	connect(button_apply, SIGNAL(clicked()), this, SLOT(compute_feature_lines()));

	connect(schnapps_, SIGNAL(object_added(Object*)), this, SLOT(object_added(Object*)));
	connect(schnapps_, SIGNAL(object_removed(Object*)), this, SLOT(object_removed(Object*)));

	schnapps_->foreach_object([this] (Object* o)
	{
		CMap2Handler* mh = qobject_cast<CMap2Handler*>(o);
		if (mh)
			map_added(mh);
	});

	plugin_cmap_provider_ = static_cast<plugin_cmap_provider::Plugin_CMapProvider*>(schnapps_->enable_plugin(plugin_cmap_provider::Plugin_CMapProvider::plugin_name()));
}

ComputeFeatureLines_Dialog::~ComputeFeatureLines_Dialog()
{
	disconnect(schnapps_, SIGNAL(object_added(Object*)), this, SLOT(object_added(Object*)));
	disconnect(schnapps_, SIGNAL(object_removed(Object*)), this, SLOT(object_removed(Object*)));
}

/*****************************************************************************/
// slots called from UI signals
/*****************************************************************************/

void ComputeFeatureLines_Dialog::selected_map_changed()
{
	if (selected_map_)
	{
		disconnect(selected_map_, SIGNAL(attribute_added(cgogn::Orbit, const QString&)), this, SLOT(selected_map_attribute_added(cgogn::Orbit, const QString&)));
		disconnect(selected_map_, SIGNAL(attribute_removed(cgogn::Orbit, const QString&)), this, SLOT(selected_map_attribute_removed(cgogn::Orbit, const QString&)));
	}

	selected_map_ = nullptr;

	QList<QListWidgetItem*> currentItems = list_maps->selectedItems();
	if (!currentItems.empty())
	{
		const QString& map_name = currentItems[0]->text();
		selected_map_ = plugin_cmap_provider_->cmap2(map_name);
	}

	if (selected_map_)
	{
		connect(selected_map_, SIGNAL(attribute_added(cgogn::Orbit, const QString&)), this, SLOT(selected_map_attribute_added(cgogn::Orbit, const QString&)));
		connect(selected_map_, SIGNAL(attribute_removed(cgogn::Orbit, const QString&)), this, SLOT(selected_map_attribute_removed(cgogn::Orbit, const QString&)));
	}

	refresh_ui();
}

void ComputeFeatureLines_Dialog::compute_feature_lines()
{
	if (selected_map_)
	{
		QString position_name = combo_positionAttribute->currentText();
		QString Kmax_name = combo_KmaxAttribute->currentText();
		QString kmax_name = combo_kmaxAttribute->currentText();
		QString Kmin_name = combo_KminAttribute->currentText();
		QString kmin_name = combo_kminAttribute->currentText();

		plugin_->compute_feature_lines(selected_map_, position_name, Kmax_name, kmax_name, Kmin_name, kmin_name);
	}
}

/*****************************************************************************/
// slots called from SCHNApps signals
/*****************************************************************************/

void ComputeFeatureLines_Dialog::object_added(Object* o)
{
	CMap2Handler* mh = qobject_cast<CMap2Handler*>(o);
	if (mh)
		map_added(mh);
}

void ComputeFeatureLines_Dialog::map_added(CMap2Handler *mh)
{
	updating_ui_ = true;
	list_maps->addItem(mh->name());
	updating_ui_ = false;
}

void ComputeFeatureLines_Dialog::object_removed(Object* o)
{
	CMap2Handler* mh = qobject_cast<CMap2Handler*>(o);
	if (mh)
		map_removed(mh);
}

void ComputeFeatureLines_Dialog::map_removed(CMap2Handler *mh)
{
	if (selected_map_ == mh)
	{
		disconnect(selected_map_, SIGNAL(attribute_added(cgogn::Orbit, const QString&)), this, SLOT(selected_map_attribute_added(cgogn::Orbit, const QString&)));
		disconnect(selected_map_, SIGNAL(attribute_removed(cgogn::Orbit, const QString&)), this, SLOT(selected_map_attribute_removed(cgogn::Orbit, const QString&)));
		selected_map_ = nullptr;
	}

	QList<QListWidgetItem*> items = list_maps->findItems(mh->name(), Qt::MatchExactly);
	if (!items.empty())
	{
		updating_ui_ = true;
		delete items[0];
		updating_ui_ = false;
	}
}

/*****************************************************************************/
// slots called from CMap2Handler signals
/*****************************************************************************/

void ComputeFeatureLines_Dialog::selected_map_attribute_added(cgogn::Orbit orbit, const QString& attribute_name)
{
	if (orbit == CMap2::Vertex::ORBIT)
	{
		QString vec3_type_name = QString::fromStdString(cgogn::name_of_type(VEC3()));
		QString float32_type_name = QString::fromStdString(cgogn::name_of_type(float32()));
		QString float64_type_name = QString::fromStdString(cgogn::name_of_type(float64()));

		const CMap2* map = selected_map_->map();
		const CMap2::ChunkArrayContainer<uint32>& container = map->attribute_container<CMap2::Vertex::ORBIT>();
		QString attribute_type_name = QString::fromStdString(container.get_chunk_array(attribute_name.toStdString())->type_name());

		if (attribute_type_name == vec3_type_name)
		{
			updating_ui_ = true;
			combo_positionAttribute->addItem(attribute_name);
			combo_KmaxAttribute->addItem(attribute_name);
			combo_KminAttribute->addItem(attribute_name);
			updating_ui_ = false;
		}

		if (attribute_type_name == float32_type_name || attribute_type_name == float64_type_name)
		{
			updating_ui_ = true;
			combo_kmaxAttribute->addItem(attribute_name);
			combo_kminAttribute->addItem(attribute_name);
			updating_ui_ = false;
		}
	}
}

void ComputeFeatureLines_Dialog::selected_map_attribute_removed(cgogn::Orbit orbit, const QString& attribute_name)
{
	if (orbit == CMap2::Vertex::ORBIT)
	{
		QString vec3_type_name = QString::fromStdString(cgogn::name_of_type(VEC3()));
		QString float32_type_name = QString::fromStdString(cgogn::name_of_type(float32()));
		QString float64_type_name = QString::fromStdString(cgogn::name_of_type(float64()));

		const CMap2* map = selected_map_->map();
		const CMap2::ChunkArrayContainer<uint32>& container = map->attribute_container<CMap2::Vertex::ORBIT>();
		QString attribute_type_name = QString::fromStdString(container.get_chunk_array(attribute_name.toStdString())->type_name());

		if (attribute_type_name == vec3_type_name)
		{
			int index = combo_positionAttribute->findText(attribute_name, Qt::MatchExactly);
			if (index > 0)
			{
				updating_ui_ = true;
				combo_positionAttribute->removeItem(index);
				updating_ui_ = false;
			}
			index = combo_KmaxAttribute->findText(attribute_name, Qt::MatchExactly);
			if (index > 0)
			{
				updating_ui_ = true;
				combo_KmaxAttribute->removeItem(index);
				updating_ui_ = false;
			}
			index = combo_KminAttribute->findText(attribute_name, Qt::MatchExactly);
			if (index > 0)
			{
				updating_ui_ = true;
				combo_KminAttribute->removeItem(index);
				updating_ui_ = false;
			}
		}

		if (attribute_type_name == float32_type_name || attribute_type_name == float64_type_name)
		{
			int index = combo_kmaxAttribute->findText(attribute_name, Qt::MatchExactly);
			if (index > 0)
			{
				updating_ui_ = true;
				combo_kmaxAttribute->removeItem(index);
				updating_ui_ = false;
			}
			index = combo_kminAttribute->findText(attribute_name, Qt::MatchExactly);
			if (index > 0)
			{
				updating_ui_ = true;
				combo_kminAttribute->removeItem(index);
				updating_ui_ = false;
			}
		}
	}
}

void ComputeFeatureLines_Dialog::refresh_ui()
{
	updating_ui_ = true;

	combo_positionAttribute->clear();
	combo_KmaxAttribute->clear();
	combo_kmaxAttribute->clear();
	combo_KminAttribute->clear();
	combo_kminAttribute->clear();

	if (selected_map_)
	{
		const CMap2* map = selected_map_->map();

		if (map->is_embedded<CMap2::Vertex>())
		{
			QString vec3_type_name = QString::fromStdString(cgogn::name_of_type(VEC3()));
			QString float32_type_name = QString::fromStdString(cgogn::name_of_type(float32()));
			QString float64_type_name = QString::fromStdString(cgogn::name_of_type(float64()));

			const CMap2::ChunkArrayContainer<uint32>& container = map->attribute_container<CMap2::Vertex::ORBIT>();
			const std::vector<std::string>& names = container.names();
			const std::vector<std::string>& type_names = container.type_names();

			for (std::size_t i = 0u; i < names.size(); ++i)
			{
				QString name = QString::fromStdString(names[i]);
				QString type = QString::fromStdString(type_names[i]);
				if (type == vec3_type_name)
				{
					combo_positionAttribute->addItem(name);
					combo_KmaxAttribute->addItem(name);
					combo_KminAttribute->addItem(name);
				}
				if (type == float32_type_name || type == float64_type_name)
				{
					combo_kmaxAttribute->addItem(name);
					combo_kminAttribute->addItem(name);
				}
			}

			int idx = combo_positionAttribute->findText(setting_auto_load_position_attribute_);
			if (idx != -1)
				combo_positionAttribute->setCurrentIndex(idx);

			idx = combo_KmaxAttribute->findText(setting_auto_load_Kmax_attribute_);
			if (idx != -1)
				combo_KmaxAttribute->setCurrentIndex(idx);

			idx = combo_kmaxAttribute->findText(setting_auto_load_kmax_attribute_);
			if (idx != -1)
				combo_kmaxAttribute->setCurrentIndex(idx);

			idx = combo_KminAttribute->findText(setting_auto_load_Kmin_attribute_);
			if (idx != -1)
				combo_KminAttribute->setCurrentIndex(idx);

			idx = combo_kminAttribute->findText(setting_auto_load_kmin_attribute_);
			if (idx != -1)
				combo_kminAttribute->setCurrentIndex(idx);
		}
	}

	updating_ui_ = false;
}

} // namespace plugin_surface_feature_lines

} // namespace schnapps
