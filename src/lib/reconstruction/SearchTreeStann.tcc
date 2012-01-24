/* Copyright (C) 2011 Uni Osnabrück
 * This file is part of the LAS VEGAS Reconstruction Toolkit,
 *
 * LAS VEGAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LAS VEGAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */


 /*
 * SearchTreeStann.tcc 
 *
 *  Created on: 02.01.2012
 *      Author: Florian Otte
 */

// stl includes
#include <limits>

// External libraries in lssr source tree
#include "../Eigen/Dense"

// boost libraries
#include <boost/filesystem.hpp>

// lssr includes

using std::cout;
using std::endl;
using std::numeric_limits;

namespace lssr {

template<typename VertexT, typename NormalT>
   SearchTreeStann< VertexT, NormalT >::SearchTreeStann( coord3fArr points, long unsigned int &n_points, const int &kn, const int &ki, const int &kd, const bool &useRansac ) 
   {
      // Store parameters
      this->m_ki = ki;
      this->m_kn = kn;
      this->m_kd = kd;
      m_useRansac = useRansac;

      // Create Stann Kd-tree
      cout << timestamp << "Creating STANN Kd-Tree" << endl;
      m_pointTree = sfcnn< coord< float >, 3, float >( points.get(), n_points, omp_get_num_procs() );
   }


template<typename VertexT, typename NormalT>
   SearchTreeStann< VertexT, NormalT >::~SearchTreeStann() { 
   }


template<typename VertexT, typename NormalT>
   void SearchTreeStann< VertexT, NormalT >::kSearch( coord< float > &qp, int neighbours, vector< ulong > &indices, vector< double > &distances )
   {
      m_pointTree.ksearch( qp, neighbours, indices, distances, 0);
   }


/*
   Begin of radiusSearch implementations
*/
template<typename VertexT, typename NormalT>
   void SearchTreeStann< VertexT, NormalT >::radiusSearch( float qp[3], double r, vector< ulong > &indices )
   {
      // clear possibly old information
      indices.clear();
      // keep track of found distances and indices
      vector< double > distances;

      double squared_radius = r*r;
      double max_radius = numeric_limits< double >::min();
      int k = 10;
      while( max_radius < r ){
         SearchTree< VertexT, NormalT >::kSearch( qp, k, indices, distances );

         // check distances for all neighbours
         for( int i=0; i < distances.size(); i++ )
         {
            max_radius = (max_radius > distances[i]) ? max_radius : distances[i];
            if( distances[i] < r )
            {
               indices.push_back( indices[i] );
            }
         }
         k *= 2;
      }
   }


template<typename VertexT, typename NormalT>
   void SearchTreeStann< VertexT, NormalT >::radiusSearch( VertexT& qp, double r, vector< ulong > &indices )
   {
      float qp_arr[3];
      qp_arr[0] = qp[0];
      qp_arr[1] = qp[1];
      qp_arr[2] = qp[2];
      this->radiusSearch( qp_arr, r, indices );
   }


template<typename VertexT, typename NormalT>
   void SearchTreeStann< VertexT, NormalT >::radiusSearch( const VertexT& qp, double r, vector< ulong > &indices )
   {
      float qp_arr[3];
      qp_arr[0] = qp[0];
      qp_arr[1] = qp[1];
      qp_arr[2] = qp[2];
      this->radiusSearch( qp_arr, r, indices );
   }


template<typename VertexT, typename NormalT>
   void SearchTreeStann< VertexT, NormalT >::radiusSearch( coord< float >& qp, double r, vector< ulong > &indices )
   {
      float qp_arr[3];
      qp_arr[0] = qp[0];
      qp_arr[1] = qp[1];
      qp_arr[2] = qp[2];
      this->radiusSearch( qp_arr, r, indices );
   }


template<typename VertexT, typename NormalT>
   void SearchTreeStann< VertexT, NormalT >::radiusSearch( const coord< float >& qp, double r, vector< ulong > &indices )
   {
      float qp_arr[3];
      coord< float > qpcpy = qp;
      qp_arr[0] = qpcpy[0];
      qp_arr[1] = qpcpy[1];
      qp_arr[2] = qpcpy[2];
      this->radiusSearch( qp_arr, r, indices );
   }


} // namespace lssr