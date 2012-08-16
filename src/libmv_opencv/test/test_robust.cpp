/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2009, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "test_precomp.hpp"
#include "opencv2/sfm/robust.hpp"

using namespace cv;
using namespace cvtest;
using namespace std;

template<typename T>
static void
test_fundamentalFromCorrespondences8PointRobust( void )
{
    const int n = 16;
    Mat_<T> x1(2,n);
    x1 << 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
          0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 5;

    Mat_<T> x2 = x1.clone();
    for (int i = 0; i < n; ++i)
    {
        x2(0, i) += i % 2;  // Multiple horizontal disparities.
    }
    x2(0, n - 1) = 10;
    x2(1, n - 1) = 10;      // The outlier has vertical disparity.

    Mat_<T> F;
    vector<int> inliers;
    fundamentalFromCorrespondences8PointRobust(x1, x2, 0.1, F, inliers);

//     cout << "F\n" << F << "\n";
//     cout << "INLIERS " << inliers.size() << "\n";

    // F should be 0, 0,  0,
    //             0, 0, -1,
    //             0, 1,  0
    EXPECT_NEAR(0.0, F(0,0), 1e-8);
    EXPECT_NEAR(0.0, F(0,1), 1e-8);
    EXPECT_NEAR(0.0, F(0,2), 1e-8);
    EXPECT_NEAR(0.0, F(1,0), 1e-8);
    EXPECT_NEAR(0.0, F(1,1), 1e-8);
    EXPECT_NEAR(0.0, F(2,0), 1e-8);
    EXPECT_NEAR(0.0, F(2,2), 1e-8);
    EXPECT_NEAR(F(1,2), -F(2,1), 1e-8);

    EXPECT_EQ(n - 1, inliers.size());
}

TEST(Sfm_robust, fundamentalFromCorrespondences8PointRobust)
{
//     test_fundamentalFromCorrespondences8PointRobust<float>();
    test_fundamentalFromCorrespondences8PointRobust<double>();
}