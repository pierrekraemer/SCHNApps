/*******************************************************************************
* SCHNApps                                                                     *
* Copyright (C) 2016, IGG Group, ICube, University of Strasbourg, France       *
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

#ifndef SCHNAPPS_PLUGIN_SHALLOW_WATER_H_
#define SCHNAPPS_PLUGIN_SHALLOW_WATER_H_

#include "dll.h"
#include <schnapps/core/plugin_processing.h>
#include <schnapps/core/map_handler.h>

#include <shallow_water_dock_tab.h>

namespace schnapps
{

namespace plugin_shallow_water
{

/**
* @brief Shallow water simulation
*/
class SCHNAPPS_PLUGIN_SHALLOW_WATER_API Plugin_ShallowWater : public PluginProcessing
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "SCHNApps.Plugin")
	Q_INTERFACES(schnapps::Plugin)

public:

	Plugin_ShallowWater() {}
	~Plugin_ShallowWater() override {}

private:

	bool enable() override;
	void disable() override;

public slots:

	void init();
	void start();
	void stop();
	bool is_running();

private slots:

	void execute_time_step();
	void try_subdivision();
	void try_simplification();
	void subdivide_face(CMap2::Face f);
	void remove_edge(CMap2::Edge e);

private:

	struct Flux
	{
		SCALAR F1;
		SCALAR F2;
		SCALAR S0L;
		SCALAR S0R;
	};

	struct Flux Solv_HLL(
		SCALAR zbL, SCALAR zbR,
		SCALAR PhiL, SCALAR PhiR,
		SCALAR hL, SCALAR hR,
		SCALAR qL, SCALAR qR,
		SCALAR hmin, SCALAR g
	);

	std::pair<CMap2::Edge, CMap2::Edge> get_LR_edges(CMap2::Face f);
	std::pair<CMap2::Face, CMap2::Face> get_LR_faces(CMap2::Edge e);

    void calcul_cste_solution_exacte();
    void solution_exacte(SCALAR x, SCALAR& h, SCALAR& q);
    void mesure_ecart();
    void optimal_dt(SCALAR dt_in);

	ShallowWater_DockTab* dock_tab_;

	SCALAR t_;
	SCALAR dt_;

    SCALAR hR;
    SCALAR hL;
    SCALAR uR;
    SCALAR uL;
    SCALAR err_h_2;
    SCALAR err_q_2;
    SCALAR err_h_max;
    SCALAR err_q_max;
    SCALAR hEtoile;
    SCALAR uEtoile;
    SCALAR ecart_h;
    SCALAR ecart_q;
    unsigned int nbr_mailles;
    SCALAR dt_max;
    const SCALAR g = 9.81;
    unsigned int nbr_iterations_temps;

	QTimer* timer_;
	bool connectivity_changed_;

	CMap2Handler* map_;
	CMap2* map2_;
	CMap2::Edge boundaryL_, boundaryR_;

	CMap2::VertexAttribute<VEC3> position_; // vertices position
    CMap2::VertexAttribute<VEC3> water_position_;
    CMap2::VertexAttribute<SCALAR> scalar_value_water_position_;
    CMap2::VertexAttribute<SCALAR> scalar_value_flow_velocity_;
    CMap2::VertexAttribute<VEC3> flow_velocity_;

	CMap2::FaceAttribute<SCALAR> h_;        // water height
	CMap2::FaceAttribute<SCALAR> h_tmp_;
	CMap2::FaceAttribute<SCALAR> q_;        // water flow
	CMap2::FaceAttribute<SCALAR> q_tmp_;
	CMap2::FaceAttribute<VEC3> centroid_;   // cell centroid
	CMap2::FaceAttribute<SCALAR> length_;   // cell length
	CMap2::FaceAttribute<SCALAR> phi_;      // cell width    

	CMap2::FaceAttribute<uint32> subd_code_;// subdivision code

    CMap2::FaceAttribute<SCALAR> erreur_h_;
    CMap2::FaceAttribute<SCALAR> erreur_q_;

	CMap2::EdgeAttribute<SCALAR> f1_;
	CMap2::EdgeAttribute<SCALAR> f2_;
	CMap2::EdgeAttribute<SCALAR> s0L_;
	CMap2::EdgeAttribute<SCALAR> s0R_;
};

} // namespace plugin_shallow_water

} // namespace schnapps

#endif // SCHNAPPS_PLUGIN_SHALLOW_WATER_H_
