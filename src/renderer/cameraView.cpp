#include "StarletGraphics/renderer/cameraView.hpp"
#include "StarletMath/constants.hpp"

namespace Starlet::Graphics {
  CameraView CameraView::fromTransform(const Math::Vec3<float>& position, const Math::Vec3<float>& rotation, const Math::Vec3<float>& worldUp) {
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
}