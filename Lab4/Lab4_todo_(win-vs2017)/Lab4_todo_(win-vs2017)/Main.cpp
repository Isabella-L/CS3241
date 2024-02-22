//============================================================
// STUDENT NAME: Lu Sicheng
// NUS User ID.: E0564634
// COMMENTS TO GRADER:
//
// ============================================================

#include "Camera.h"
#include "Color.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Ray.h"
#include "Raytrace.h"
#include "Scene.h"
#include "Sphere.h"
#include "Surface.h"
#include "Triangle.h"
#include "Util.h"
#include "Vector3d.h"
#include <string>

// Constants for Scene 1.
static constexpr int imageWidth1 = 640;
static constexpr int imageHeight1 = 480;
static constexpr int reflectLevels1 = 1; // 0 -- object does not reflect scene.
static constexpr int hasShadow1 = true;
static constexpr std::string_view outImageFile1 = "out1.png";

// Constants for Scene 2.
static constexpr int imageWidth2 = 640;
static constexpr int imageHeight2 = 480;
static constexpr int reflectLevels2 = 2; // 0 -- object does not reflect scene.
static constexpr int hasShadow2 = true;
static constexpr std::string_view outImageFile2 = "out2.png";

///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage(const std::string &imageFilename, const Scene &scene,
                 int reflectLevels, bool hasShadow) {
  int imgWidth = scene.camera.getImageWidth();
  int imgHeight = scene.camera.getImageHeight();

  Image image(imgWidth, imgHeight); // To store the result of ray tracing.

  double startTime = Util::GetCurrRealTime();
  double startCPUTime = Util::GetCurrCPUTime();

// Generate image, rendering in parallel on Windows and Linux.
#ifndef __APPLE__
#pragma warning(push)
#pragma warning(disable : 6993)
#pragma omp parallel for
#endif
  for (int y = 0; y < imgHeight; y++) {
    double pixelPosY = y + 0.5;

    for (int x = 0; x < imgWidth; x++) {
      double pixelPosX = x + 0.5;
      Ray ray = scene.camera.getRay(pixelPosX, pixelPosY);
      Color pixelColor =
          Raytrace::TraceRay(ray, scene, reflectLevels, hasShadow);
      pixelColor.clamp();
      image.setPixel(x, y, pixelColor);
    }
  }
#ifndef __APPLE__
#pragma warning(pop)
#endif

  double cpuTimeElapsed = Util::GetCurrCPUTime() - startCPUTime;
  double realTimeElapsed = Util::GetCurrRealTime() - startTime;
  std::cout << "CPU time taken = " << cpuTimeElapsed << "sec" << std::endl;
  std::cout << "Real time taken = " << realTimeElapsed << "sec" << std::endl;

  // Write image to file.
  if (!image.writeToFile(imageFilename))
    return;
  else
    Util::ErrorExit("File: %s could not be written.\n", imageFilename.c_str());
}

// Forward declarations. These functions are defined later in the file.
void DefineScene1(Scene &scene, int imageWidth, int imageHeight);
void DefineScene2(Scene &scene, int imageWidth, int imageHeight);

int main() {
  // Define Scene 1.

  Scene scene1;
  DefineScene1(scene1, imageWidth1, imageHeight1);

  // Render Scene 1.

  std::cout << "Render Scene 1..." << std::endl;
  RenderImage(std::string(outImageFile1), scene1, reflectLevels1, hasShadow1);
  std::cout << "Scene 1 completed." << std::endl;

  // Delete Scene 1 surfaces.

  for (auto &surface : scene1.surfaces) {
    delete surface;
  }

  // Define Scene 2.

  Scene scene2;
  DefineScene2(scene2, imageWidth2, imageHeight2);

  // Render Scene 2.

  std::cout << "Render Scene 2..." << std::endl;
  RenderImage(std::string(outImageFile2), scene2, reflectLevels2, hasShadow2);
  std::cout << "Scene 2 completed." << std::endl;

  // Delete Scene 2 surfaces.

  for (auto &surface : scene2.surfaces) {
    delete surface;
  }

  std::cout << "All done. Press Enter to exit." << std::endl;
  std::cin.get();
  return 0;
}

///////////////////////////////////////////////////////////////////////////
// Modelling of Scene 1.
///////////////////////////////////////////////////////////////////////////

void DefineScene1(Scene &scene, int imageWidth, int imageHeight) {
  scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

  scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

  // Define materials.

  // Light red.
  Material lightRed = Material();
  lightRed.k_d = Color(0.8f, 0.4f, 0.4f);
  lightRed.k_a = lightRed.k_d;
  lightRed.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  lightRed.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  lightRed.n = 64.0f;

  // Light green.
  Material lightGreen = Material();
  lightGreen.k_d = Color(0.4f, 0.8f, 0.4f);
  lightGreen.k_a = lightGreen.k_d;
  lightGreen.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  lightGreen.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  lightGreen.n = 64.0f;

  // Light blue.
  Material lightBlue = Material();
  lightBlue.k_d = Color(0.4f, 0.4f, 0.8f) * 0.9f;
  lightBlue.k_a = lightBlue.k_d;
  lightBlue.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  lightBlue.k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
  lightBlue.n = 64.0f;

  // Yellow.
  Material yellow = Material();
  yellow.k_d = Color(0.6f, 0.6f, 0.2f);
  yellow.k_a = yellow.k_d;
  yellow.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  yellow.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  yellow.n = 64.0f;

  // Gray.
  Material gray = Material();
  gray.k_d = Color(0.6f, 0.6f, 0.6f);
  gray.k_a = gray.k_d;
  gray.k_r = Color(0.6f, 0.6f, 0.6f);
  gray.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  gray.n = 128.0f;

  // Insert into scene materials vector.
  scene.materials = {lightRed, lightGreen, lightBlue, yellow, gray};

  // Define point light sources.

  scene.ptLights.resize(2);

  PointLightSource light0 = {Vector3d(100.0, 120.0, 10.0),
                             Color(1.0f, 1.0f, 1.0f) * 0.6f};
  PointLightSource light1 = {Vector3d(5.0, 80.0, 60.0),
                             Color(1.0f, 1.0f, 1.0f) * 0.6f};

  scene.ptLights = {light0, light1};

  // Define surface primitives.

  scene.surfaces.resize(15);

  auto horzPlane =
      new Plane(0.0, 1.0, 0.0, 0.0, scene.materials[2]); // Horizontal plane.
  auto leftVertPlane =
      new Plane(1.0, 0.0, 0.0, 0.0, scene.materials[4]); // Left vertical plane.
  auto rightVertPlane = new Plane(0.0, 0.0, 1.0, 0.0,
                                  scene.materials[4]); // Right vertical plane.
  auto bigSphere = new Sphere(Vector3d(40.0, 20.0, 42.0), 22.0,
                              scene.materials[0]); // Big sphere.
  auto smallSphere = new Sphere(Vector3d(75.0, 10.0, 40.0), 12.0,
                                scene.materials[1]); // Small sphere.

  // Cube +y face.
  auto cubePosYTri1 =
      new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(50.0, 20.0, 70.0),
                   Vector3d(30.0, 20.0, 70.0), scene.materials[3]);
  auto cubePosYTri2 =
      new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(30.0, 20.0, 70.0),
                   Vector3d(30.0, 20.0, 90.0), scene.materials[3]);

  // Cube +x face.
  auto cubePosXTri1 =
      new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
                   Vector3d(50.0, 20.0, 90.0), scene.materials[3]);
  auto cubePosXTri2 =
      new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 90.0),
                   Vector3d(50.0, 0.0, 90.0), scene.materials[3]);

  // Cube -x face.
  auto cubeNegXTri1 =
      new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
                   Vector3d(30.0, 20.0, 70.0), scene.materials[3]);
  auto cubeNegXTri2 =
      new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 70.0),
                   Vector3d(30.0, 0.0, 70.0), scene.materials[3]);

  // Cube +z face.
  auto cubePosZTri1 =
      new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(50.0, 20.0, 90.0),
                   Vector3d(30.0, 20.0, 90.0), scene.materials[3]);
  auto cubePosZTri2 =
      new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
                   Vector3d(30.0, 0.0, 90.0), scene.materials[3]);

  // Cube -z face.
  auto cubeNegZTri1 =
      new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(30.0, 20.0, 70.0),
                   Vector3d(50.0, 20.0, 70.0), scene.materials[3]);
  auto cubeNegZTri2 =
      new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
                   Vector3d(50.0, 0.0, 70.0), scene.materials[3]);

  scene.surfaces = {horzPlane,    leftVertPlane, rightVertPlane, bigSphere,
                    smallSphere,  cubePosXTri1,  cubePosXTri2,   cubePosYTri1,
                    cubePosYTri2, cubePosZTri1,  cubePosZTri2,   cubeNegXTri1,
                    cubeNegXTri2, cubeNegZTri1,  cubeNegZTri2};

  // Define camera.

  scene.camera = Camera(Vector3d(150.0, 120.0, 150.0),     // eye
                        Vector3d(45.0, 22.0, 55.0),        // lookAt
                        Vector3d(0.0, 1.0, 0.0),           // upVector
                        (-1.0 * imageWidth) / imageHeight, // left
                        (1.0 * imageWidth) / imageHeight,  // right
                        -1.0, 1.0, 3.0,                    // bottom, top, near
                        imageWidth, imageHeight); // image_width, image_height
}

///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////

void MakeTanghulu(double x1, double x2, double x3, double y_pos, double y_neg,
                  double z_pos, double z_neg, int num_of_sphere, Scene &scene) {

  // long stick
  // top and bottom
  auto stickPosYTri =
      new Triangle(Vector3d(x1, y_pos, z_pos),
                   Vector3d(x2, y_pos, z_neg), // y stays the same
                   Vector3d(x3, y_pos, z_pos), scene.materials[3]);
  auto stickNegYTri =
      new Triangle(Vector3d(x1, y_neg, z_pos),
                   Vector3d(x2, y_neg, z_neg), // y stays the same
                   Vector3d(x3, y_neg, z_pos), scene.materials[3]);
  // sides
  auto stickFrontTri1 =
      new Triangle(Vector3d(x3, y_pos, z_pos), Vector3d(x2, y_pos, z_neg),
                   Vector3d(x3, y_neg, z_pos), scene.materials[3]);
  auto stickFrontTri2 =
      new Triangle(Vector3d(x2, y_pos, z_neg), Vector3d(x2, y_neg, z_neg),
                   Vector3d(x3, y_neg, z_pos), scene.materials[3]);

  auto stickBackTri1 =
      new Triangle(Vector3d(x1, y_pos, z_pos), Vector3d(x2, y_pos, z_neg),
                   Vector3d(x1, y_neg, z_pos), scene.materials[3]);
  auto stickBackTri2 =
      new Triangle(Vector3d(x1, y_neg, z_pos), Vector3d(x2, y_pos, z_neg),
                   Vector3d(x2, y_neg, z_neg), scene.materials[3]);

  auto stickSideTri1 =
      new Triangle(Vector3d(x1, y_pos, z_pos), Vector3d(x3, y_pos, z_pos),
                   Vector3d(x1, y_neg, z_pos), scene.materials[3]);
  auto stickSideTri2 =
      new Triangle(Vector3d(x1, y_neg, z_pos), Vector3d(x3, y_pos, z_pos),
                   Vector3d(x3, y_neg, z_pos), scene.materials[3]);
  scene.surfaces.push_back(stickPosYTri);
  scene.surfaces.push_back(stickNegYTri);
  scene.surfaces.push_back(stickFrontTri1);
  scene.surfaces.push_back(stickBackTri1);
  scene.surfaces.push_back(stickBackTri2);
  scene.surfaces.push_back(stickSideTri1);
  scene.surfaces.push_back(stickSideTri2);

  for (int i = 0; i < num_of_sphere; i++) {
    double radius = 5.0;
    auto sphere =
        new Sphere(Vector3d(x2, 25 + i * radius * 2, (z_pos + z_neg) / 2),
                   radius, scene.materials[0]);
    scene.surfaces.push_back(sphere);
  }
}

void DefineScene2(Scene &scene, int imageWidth, int imageHeight) {
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  // must use all surface primitives types.
  scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);
  scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.5f;

  // Define materials.

  // Light red.
  Material lightRed = Material();
  lightRed.k_d = Color(0.9f, 0.4f, 0.4f);
  lightRed.k_a = lightRed.k_d;
  lightRed.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  lightRed.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  lightRed.n = 64.0f;

  // Light green.
  Material lightGreen = Material();
  lightGreen.k_d = Color(0.4f, 0.8f, 0.4f);
  lightGreen.k_a = lightGreen.k_d;
  lightGreen.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  lightGreen.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  lightGreen.n = 64.0f;

  // Light blue.
  Material lightBlue = Material();
  lightBlue.k_d = Color(0.4f, 0.4f, 0.8f) * 0.9f;
  lightBlue.k_a = lightBlue.k_d;
  lightBlue.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  lightBlue.k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
  lightBlue.n = 64.0f;

  // Yellow.
  Material brown = Material();
  brown.k_d = Color(0.7f, 0.8f, 0.3f);
  brown.k_a = brown.k_d;
  brown.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  brown.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.5f;
  brown.n = 64.0f;

  // Gray.
  Material gray = Material();
  gray.k_d = Color(0.6f, 0.6f, 0.6f);
  gray.k_a = gray.k_d;
  gray.k_r = Color(0.6f, 0.6f, 0.6f);
  gray.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  gray.n = 128.0f;

  // Insert into scene materials vector.
  scene.materials = {lightRed, lightGreen, lightBlue, brown, gray};

  // Define point light sources.

  scene.ptLights.resize(2);

  PointLightSource light0 = {Vector3d(100.0, 120.0, 10.0),
                             Color(1.0f, 1.0f, 1.0f) * 0.6f};
  PointLightSource light1 = {Vector3d(5.0, 80.0, 60.0),
                             Color(1.0f, 1.0f, 1.0f) * 0.6f};

  scene.ptLights = {light0, light1};

  // Define surface primitives.

  scene.surfaces.resize(15);

  auto horzPlane =
      new Plane(0.0, 1.0, 0.0, 0.0, scene.materials[4]); // Horizontal plane.
  auto leftVertPlane =
      new Plane(1.0, 0.0, 0.0, 0.0, scene.materials[1]); // Left vertical plane.
  auto rightVertPlane = new Plane(0.0, 0.0, 1.0, 0.0,
                                  scene.materials[2]); // Right vertical plane.

  scene.surfaces = {horzPlane, leftVertPlane, rightVertPlane};

  double x1 = 44.0;
  double x2 = 45.0;
  double x3 = 46.0;
  double y_pos = 70.0;
  double y_neg = 5.0;
  double z_pos = 60.0;
  double z_neg = 62.0;

  MakeTanghulu(x1, x2, x3, y_pos, y_neg, z_pos, z_neg, 4, scene);
  MakeTanghulu(60.0, 61.0, 62.0, y_pos - 10.0, y_neg, 35.0, 37.0, 3, scene);
  MakeTanghulu(50.0, 51.0, 52.0, y_pos + 3.0, y_neg, 100.0, 102.0, 5, scene);
  MakeTanghulu(30.0, 31.0, 32.0, y_pos - 10.0, y_neg, 70.0, 72.0, 1, scene);
  MakeTanghulu(25.0, 26.0, 27.0, y_pos - 10.0, y_neg, 10.0, 12.0, 2, scene);
  // Define camera.

  scene.camera = Camera(Vector3d(150.0, 60.0, 150.0),      // eye
                        Vector3d(45.0, 22.0, 55.0),        // lookAt
                        Vector3d(0.0, 1.0, 0.0),           // upVector
                        (-1.0 * imageWidth) / imageHeight, // left
                        (1.0 * imageWidth) / imageHeight,  // right
                        -1.0, 1.0, 3.0,                    // bottom, top, near
                        imageWidth, imageHeight);
}