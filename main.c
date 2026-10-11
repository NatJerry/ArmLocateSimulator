#include <math.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((useconds_t)((ms) * 1000))
#endif

int main(void) {
    double testCollection[6][3] = {
        {12, 0, 16}, {10, 10, 20}, {0, 0, 20}, {22, 0, 10}, {25, 0, 15}, {5, 5, 2}
    };
    for (int i = 0; i < 6; i++) {
        //-pi<J1<=pi; -pi/2<J2<pi/2; -pi<J3<=pi;
        double x = testCollection[i][0], y = testCollection[i][1], z = testCollection[i][2];
        double distance = sqrt(x * x + y * y + (z - 10) * (z - 10));
        if (distance >= 2 && distance <= 22) {
            double J1 = atan2(y, x);
            double h = z - 10;
            double dis = sqrt(x * x + y * y);
            double delta = atan2(h, dis);
            double cos_alpha = (12 * 12 + distance * distance - 10 * 10) / (2 * 12 * distance);
            if (cos_alpha > 1.0) cos_alpha = 1.0;
            if (cos_alpha < -1.0) cos_alpha = -1.0;
            //防止浮点误差
            double alpha = acos(cos_alpha);
            double J2, J3;
            if (delta - alpha >= -M_PI / 2 && delta - alpha <= M_PI / 2) {
                J2 = delta - alpha;
                double J3_dis = dis - 12 * cos(J2);
                double J3_h = h - 12 * sin(J2);
                J3 = atan2(J3_h, J3_dis);
            } else if (delta + alpha >= -M_PI / 2 && delta + alpha <= M_PI / 2) {
                J2 = delta + alpha;
                double J3_dis = dis - 12 * cos(J2);
                double J3_h = h - 12 * sin(J2);
                J3 = atan2(J3_h, J3_dis);
            } else {
                printf("不可达，当前点位在当前限位下不可达.\n");
                continue;
            }
            J1 = J1 / M_PI * 180;
            J2 = J2 / M_PI * 180;
            J3 = J3 / M_PI * 180;
            double animated_J1 = 0, animated_J2 = 0, animated_J3 = 0;
            double x1 = 0, y1 = 0, z1 = 0, d1 = 0;
            for (int j = 1; j <= 50; j++) {
                double interval = (double) j / 50;
                animated_J1 = interval * J1;
                animated_J2 = interval * J2;
                animated_J3 = interval * J3;
                d1 = 12 * cos(animated_J2 / 180 * M_PI) + 10 * cos(animated_J3 / 180 * M_PI);
                x1 = d1 * cos(animated_J1 / 180 * M_PI);
                y1 = d1 * sin(animated_J1 / 180 * M_PI);
                z1 = 12 * sin(animated_J2 / 180 * M_PI) + 10 * sin(animated_J3 / 180 * M_PI) + 10;
                printf("Frame %d,Angle %f %f %f,Location %f %f %f \n", j, animated_J1, animated_J2, animated_J3, x1, y1,
                       z1);
                sleep_ms(50);
            }
            printf("%f,%f,%f (%f,%f,%f)\n", J1, J2, J3, x, y, z);
        } else if (distance < 2) {
            printf("不可达，未达工作空间范围\n");
        } else {
            printf("不可达，超出工作空间范围\n");
        }
        printf("按回车继续");
        getchar();
    }
    return 0;
}
