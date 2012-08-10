// __BEGIN_LICENSE__
//  Copyright (c) 2009-2012, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NGT platform is licensed under the Apache License, Version 2.0 (the
//  "License"); you may not use this file except in compliance with the
//  License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__

#include <asp/Sessions/RPC/StereoSessionRPC.h>
#include <asp/Sessions/RPC/RPCModel.h>
#include <asp/Sessions/DG/XML.h>
#include <test/Helpers.h>

using namespace vw;
using namespace asp;
using namespace xercesc;

TEST( StereoSessionRPC, InstantiateTest ) {
  XMLPlatformUtils::Initialize();

  // Create an RPC Model
  RPCXML xml;
  xml.read_from_file( "dg_example1.xml" );
  RPCModel model( *xml.rpc_ptr() );

  // Verify some of the values
  EXPECT_NEAR( 4.683662e-3, model.line_num_coeff()[0], 1e-6 );
  EXPECT_NEAR( 1, model.line_den_coeff()[0], 1e-6 );
  EXPECT_NEAR( -7.306375e-3, model.sample_num_coeff()[0], 1e-6 );
  EXPECT_NEAR( 1, model.sample_den_coeff()[0], 1e-6 );
  EXPECT_VECTOR_NEAR( Vector2(17564,11856), model.xy_offset(), 1e-3 );
  EXPECT_VECTOR_NEAR( Vector2(17927,12384), model.xy_scale(), 1e-3 );

  // Verify that nothing segfaults or has a run time error.
  Vector3 location(-105.29,39.745,2281);
  EXPECT_NO_THROW( model.calculate_terms( location ) );
  EXPECT_NO_THROW( model.terms_Jacobian( location ) );
  EXPECT_NO_THROW( model.normalization_Jacobian( location ) );
  EXPECT_NO_THROW( model.geodetic_to_pixel_Jacobian( location ) );
  EXPECT_NO_THROW( model.geodetic_to_pixel( location ) );
  EXPECT_THROW( model.pixel_to_vector( Vector2() ), vw::NoImplErr );
  EXPECT_THROW( model.camera_center( Vector2() ), vw::NoImplErr );

  XMLPlatformUtils::Terminate();
}