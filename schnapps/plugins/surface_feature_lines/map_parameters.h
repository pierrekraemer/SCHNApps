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

#ifndef SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_MAP_PARAMETERS_H_
#define SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_MAP_PARAMETERS_H_

#include <schnapps/plugins/surface_feature_lines/dll.h>

#include <schnapps/plugins/cmap_provider/cmap_provider.h>

#include <schnapps/core/types.h>
#include <schnapps/core/view.h>

#include <cgogn/rendering/shaders/shader_bold_line.h>

namespace schnapps
{

namespace plugin_surface_feature_lines
{

class Plugin_SurfaceFeatureLines;

using CMap2Handler = plugin_cmap_provider::CMap2Handler;

struct SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_API MapParameters : public QObject
{
	Q_OBJECT

	friend class Plugin_SurfaceFeatureLines;

public:

	MapParameters() :
		mh_(nullptr),
		shader_bold_line_param_feature_lines_(nullptr),
		feature_lines_vbo_(nullptr),
		nb_feature_lines_(0)
	{
		feature_lines_vbo_ = new cgogn::rendering::VBO(3);
		initialize_gl();
	}

	~MapParameters()
	{
		delete feature_lines_vbo_;
	}

	CGOGN_NOT_COPYABLE_NOR_MOVABLE(MapParameters);

	uint32 nb_feature_lines() const { return nb_feature_lines_; }

private:

	void update_lines(const std::vector<VEC3>& line_segments)
	{
		nb_feature_lines_ = line_segments.size() / 2;
		cgogn::rendering::update_vbo(line_segments, feature_lines_vbo_);
	}

	void initialize_gl()
	{
		shader_bold_line_param_feature_lines_ = cgogn::rendering::ShaderBoldLine::generate_param();
		shader_bold_line_param_feature_lines_->color_ = QColor(255, 0, 0);
		shader_bold_line_param_feature_lines_->width_ = 2.0f;
		shader_bold_line_param_feature_lines_->set_position_vbo(feature_lines_vbo_);
	}

private:

	CMap2Handler* mh_;

	std::unique_ptr<cgogn::rendering::ShaderBoldLine::Param> shader_bold_line_param_feature_lines_;
	cgogn::rendering::VBO* feature_lines_vbo_;
	uint32 nb_feature_lines_;
};

} // namespace plugin_surface_feature_lines

} // namespace schnapps

#endif // SCHNAPPS_PLUGIN_SURFACE_FEATURE_LINES_MAP_PARAMETERS_H_
