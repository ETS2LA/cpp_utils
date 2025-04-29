import sys
sys.path.append("./ets2la_AR")

import ets2la_AR


x = 6999.413571043637
y = 8.518050886553258
z = -19635.664071664563
cam_x = 6980.038009643555
cam_y = 11.304534912109375
cam_z = -19631.72882080078
cam_pitch = -33.0555179448936
cam_yaw = -110.97706981961045
cam_roll = -1.0647704063119583
cam_fov = 80.80000305175781
window_x = 9
window_y = 42
window_width = 1289
window_height = 762

result = ets2la_AR.game_to_screen_coordinate(x, y, z, cam_x, cam_y, cam_z, cam_pitch, cam_yaw, cam_roll, cam_fov, window_x, window_y, window_width, window_height, False, False)

print(result)