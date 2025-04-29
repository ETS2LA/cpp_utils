#include <pybind11/pybind11.h>
#include <numbers>

namespace py = pybind11;

constexpr double deg2rad(double deg) {
    return deg * std::numbers::pi / 180.0;
}

py::object game_to_screen_coordinate(double x, double y, double z, double cam_x, double cam_y, double cam_z, double head_x, double head_y, double head_z, double cam_pitch, double cam_yaw, double cam_roll, double cabin_pitch, double cabin_yaw, double cabin_roll, double cam_fov, int window_x, int window_y, int window_width, int window_height, bool coordinate_relative, bool coordinate_rotation_relative) {
    double cos_pitch;
    double sin_pitch;
    double cos_yaw;
    double sin_yaw;
    double cos_roll;
    double sin_roll;

    double new_x;
    double new_y;
    double new_z;
    double final_x;
    double final_y;
    double final_z;
    double relative_x;
    double relative_y;
    double relative_z;

    if (coordinate_relative) {
        relative_x = x;
        relative_y = y;
        relative_z = z;

        // if the cam is not the truck inside cam, hide everything with relative coordinates
        if (abs(cam_x - head_x) > 1 || abs(cam_y - head_y) > 1 || abs(cam_z - head_z) > 1)
            return py::make_tuple(py::none(), py::none(), py::none());

        if (coordinate_rotation_relative) {
            cos_pitch = cos(deg2rad(cabin_pitch));
            sin_pitch = sin(deg2rad(cabin_pitch));
            cos_yaw = cos(deg2rad(cabin_yaw));
            sin_yaw = sin(deg2rad(cabin_yaw));
            cos_roll = cos(0);
            sin_roll = sin(0);

            new_y = relative_y * cos_pitch - relative_z * sin_pitch;
            new_z = relative_z * cos_pitch + relative_y * sin_pitch;

            new_x = relative_x * cos_yaw + new_z * sin_yaw;
            new_z = new_z * cos_yaw - relative_x * sin_yaw;

            final_x = new_x * cos_roll - new_y * sin_roll;
            final_y = new_y * cos_roll + new_x * sin_roll;

            relative_x = final_x;
            relative_y = final_y;
            relative_z = new_z;
        }
    } else {
        relative_x = x - cam_x;
        relative_y = y - cam_y;
        relative_z = z - cam_z;
    }

    cos_pitch = cos(deg2rad(-cam_pitch));
    sin_pitch = sin(deg2rad(-cam_pitch));
    cos_yaw = cos(deg2rad(-cam_yaw));
    sin_yaw = sin(deg2rad(-cam_yaw));
    cos_roll = cos(deg2rad(-cam_roll));
    sin_roll = sin(deg2rad(-cam_roll));

    new_x = relative_x * cos_yaw + relative_z * sin_yaw;
    new_z = relative_z * cos_yaw - relative_x * sin_yaw;

    new_y = relative_y * cos_pitch - new_z * sin_pitch;
    final_z = new_z * cos_pitch + relative_y * sin_pitch;

    final_x = new_x * cos_roll - new_y * sin_roll;
    final_y = new_y * cos_roll + new_x * sin_roll;

    // point behind the camera
    if (final_z >= 0) {
        return py::make_tuple(py::none(), py::none(), py::none());
    }

    double fov_rad = deg2rad(cam_fov);
    if (fov_rad <= 0) {
        return py::make_tuple(py::none(), py::none(), py::none());
    }

    double window_distance = ((window_height - window_y) * (4.0 / 3.0) / 2.0) / tan(fov_rad / 2.0); // important to do .0

    double screen_x = (final_x / final_z) * window_distance + (window_width - window_x) / 2.0;
    double screen_y = (final_y / final_z) * window_distance + (window_height - window_y) / 2.0;

    screen_x = (window_width - window_x) - screen_x;

    double distance = sqrt(relative_x * relative_x + relative_y * relative_y + relative_z * relative_z);

    return py::make_tuple(screen_x, screen_y, distance);
}

PYBIND11_MODULE(ets2la_AR, m) {
    m.def("game_to_screen_coordinate", &game_to_screen_coordinate, "Converts the 3D game coordinate to the 2D screen coordinate.");
}