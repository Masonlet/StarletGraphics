#include "StarletGraphics/renderer/cameraView.hpp"
#include "StarletMath/constants.hpp"

CameraView CameraView::fromTransform(const Vec3<float>& position, const Vec3<float>& rotation, const Vec3<float>& worldUp) {
  CameraView view;

  view.eye = position;

  const float yaw = rotation.y;
  const float pitch = rotation.x;
  view.front.x = std::cos(radians(yaw)) * std::cos(radians(pitch));
  view.front.y = std::sin(radians(pitch));
  view.front.z = std::sin(radians(yaw)) * std::cos(radians(pitch));
  view.front = view.front.normalized();

  view.right = view.front.cross(worldUp).normalized();
  view.up = view.right.cross(view.front).normalized();
  return view;
}