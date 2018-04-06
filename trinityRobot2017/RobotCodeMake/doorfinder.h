#include "globals.h"
#include "constants.h"
#include "point.h"
#include "occupancygrid.h"

#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <deque>
#include <map>

using std::vector;
using std::map;
using std::deque;

struct cluster{
    DoublePoint averagePoint;
    float averageAngle;
    int size;
    int hallwayCount;
};

struct DH{
    int angle;
    DoublePoint point;
    float dist;
    bool isHallway;
};

class DoorFinder
{
    public:
        /** Default constructor */
        DoorFinder(OccupancyGrid o);
        vector<int> average(deque<int> scan, int numSections);
        map<int, vector<Point>> findDoorsAndHallways(std::deque<int> scan, map<int, vector<Point>> targetPoints);
        static DoublePoint prevPos;
    private:
        vector<int> NUM_SECTIONS = {36, 30, 24, 20, 18, 16, 15, 12, 10};
        int numIndex;
        double angleSize;
        OccupancyGrid occGrid;

        /*deque<int> testScan = {34, 34, 34, 33, 33, 33, 241, 33, 33, 33, 229, 33, 33, 33, 53, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 193, 33, 33, 33, 188, 33, 33, 33, 53,
        165, 171, 172, 172, 172, 175, 53, 172, 34, 34, 34, 34, 34, 171, 53, 0, 0, 0, 0, 33, 177, 33, 178, 33, 182, 33, 185, 33, 188, 33, 190, 33, 194, 33, 197, 33, 201, 33,
        205, 33, 210, 33, 215, 33, 222, 33, 221, 33, 231, 33, 240, 33, 246, 33, 252, 33, 260, 33, 263, 33, 258, 33, 2, 33, 244, 33, 235, 33, 227, 33, 221, 33, 214, 33, 209,
        33, 204, 33, 200, 33, 196, 33, 193, 33, 190, 33, 186, 33, 185, 33, 183, 33, 181, 33, 179, 33, 178, 33, 177, 33, 177, 33, 176, 33, 176, 33, 176, 33, 176, 33,
        176, 33, 177, 33, 178, 33, 53, 33, 53, 33, 53, 33, 184, 33, 186, 33, 188, 33, 191, 33, 190, 33, 53, 33, 161, 33, 2, 33, 210, 33, 53, 33, 217, 33, 221, 33, 230, 33, 53,
        33, 244, 33, 251, 33, 264, 33, 271, 33, 280, 33, 293, 33, 306, 33, 322, 33, 335, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53, 33, 53,
        33, 53, 33, 882, 33, 816, 33, 799, 33, 788, 33, 783, 33, 781, 33, 779, 33, 778, 33, 784, 33, 792, 33, 802, 33, 823, 33, 851, 33, 53, 33, 53, 33, 53, 33, 401, 33, 410, 33,
        421, 33, 434, 33, 451, 33, 53, 33, 53, 33, 495, 33, 521, 33, 541, 33, 561, 33, 2, 33, 540, 33, 533, 33, 53, 33, 513, 33, 497, 33, 53, 33, 53, 33, 492, 33, 486, 33, 483,
        33, 462, 33, 488, 33, 484, 33, 474, 33, 464, 33, 474, 33, 475, 33, 476, 33, 478, 33, 481, 33, 484, 33, 488, 33, 492, 33, 498, 33, 500, 33, 2, 33, 2, 33, 443, 33, 406, 33,
        373, 33, 347, 33, 325, 33, 305, 33, 289, 33, 274, 33, 262, 34, 34, 34, 34 };*/ //fake room 1

        deque<int> testScan = {34, 261, 33, 35, 34, 33, 33, 33, 248, 33, 33, 33, 243, 33, 33, 33, 239, 33, 33, 33, 235, 33, 33, 33, 233, 33, 33, 33, 231, 33,
        33, 33, 230, 33, 33, 33, 53, 2, 241, 243, 246, 249, 251, 251, 256, 34, 34, 34, 34, 34, 258, 259, 263, 265, 264, 264, 53, 53, 53, 53, 298, 297, 296, 299,
        303, 308, 313, 318, 328, 317, 309, 304, 295, 288, 283, 277, 271, 265, 261, 256, 252, 249, 245, 242, 238, 234, 231, 228, 226, 223, 221, 219, 216, 214, 212,
        210, 208, 207, 205, 204, 202, 201, 200, 199, 198, 197, 197, 197, 195, 195, 194, 194, 193, 193, 192, 192, 192, 192, 193, 195, 185, 191, 191, 192, 193, 193,
        194, 195, 195, 196, 197, 198, 199, 200, 202, 203, 205, 206, 208, 209, 211, 212, 214, 216, 219, 221, 33, 225, 227, 230, 233, 236, 240, 53, 53, 53, 53, 53,
        254, 262, 266, 271, 275, 280, 285, 290, 295, 301, 307, 313, 320, 323, 325, 53, 53, 53, 53, 53, 398, 403, 410, 421, 434, 448, 462, 479, 495, 514, 534, 555,
        579, 604, 633, 662, 701, 739, 781, 829, 881, 945, 1020, 1099, 1193, 1227, 1231, 1229, 1227, 1227, 1224, 1225, 1224, 1227, 1230, 1229, 1231, 1232, 1233, 1238,
        1240, 1244, 879, 880, 884, 888, 2, 422, 395, 371, 347, 329, 308, 292, 279, 266, 255, 244, 233, 232, 234, 236, 242, 231, 228, 231, 234, 237, 240, 242, 247,
        250, 253, 257, 261, 265, 269, 274, 279, 284, 290, 295, 301, 308, 314, 320, 328, 336, 345, 354, 362, 373, 388, 53, 53, 53, 53, 440, 459, 474, 492, 511, 531,
        551, 575, 601, 629, 657, 53, 974, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 958, 859, 791, 730, 679, 639, 603, 570, 546, 515,
        495, 474, 455, 442, 425, 411, 398, 385, 373, 363, 34, 34, 34, 34, 349, 342, 335, 329, 322, 316, 310, 305, 300, 296, 291, 287, 283, 279, 276, 272, 269,
        266, 263, 34, 34, 34, 34, 34,}; //hallway 1

        /*deque<int> testScan = {34, 736, 33, 729, 33, 723, 33, 717, 33, 712, 33, 708, 33, 703, 33, 700, 33, 698, 33, 696, 33, 695, 33, 693, 33, 692, 33, 693,
        33, 694, 33, 696, 33, 696, 33, 697, 53, 53, 380, 365, 349, 333, 320, 306, 294, 34, 34, 34, 34, 34, 286, 276, 269, 262, 252, 53, 53, 53, 241, 233, 225,
        221, 217, 213, 210, 207, 203, 200, 198, 195, 192, 190, 188, 185, 33, 182, 180, 178, 177, 175, 174, 172, 170, 169, 169, 167, 166, 165, 165, 163, 163, 162,
        162, 161, 160, 160, 160, 159, 159, 159, 159, 158, 159, 158, 159, 159, 159, 159, 159, 160, 160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 168, 33,
        170, 171, 172, 174, 175, 177, 178, 180, 181, 183, 185, 186, 188, 190, 192, 194, 197, 199, 202, 204, 207, 209, 212, 33, 219, 221, 225, 230, 232, 239, 53, 53,
        53, 53, 37, 261, 270, 277, 283, 289, 296, 303, 310, 319, 328, 337, 347, 33, 368, 376, 383, 53, 53, 53, 53, 37, 512, 529, 548, 572, 603, 636, 675, 718, 581,
        2, 775, 777, 772, 772, 770, 770, 769, 769, 771, 771, 771, 772, 772, 774, 775, 777, 778, 781, 784, 786, 787, 791, 793, 798, 800, 805, 809, 813, 818, 33, 2,
        442, 444, 448, 452, 456, 460, 465, 468, 473, 478, 484, 489, 495, 501, 507, 513, 521, 529, 536, 545, 554, 563, 572, 582, 593, 603, 616, 628, 641, 655, 671,
        687, 703, 722, 739, 754, 762, 2, 774, 767, 763, 756, 750, 742, 737, 730, 725, 721, 716, 716, 725, 53, 53, 53, 53, 682, 53, 685, 682, 680, 679, 677, 675, 673,
        673, 672, 671, 672, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 173, 175, 177, 179, 181, 183, 186, 188, 190, 192, 195, 34,
        34, 34, 34, 210, 213, 217, 220, 224, 228, 232, 236, 240, 245, 251, 256, 262, 268, 274, 281, 288, 295, 53, 53, 53, 53,
        753, 738, 732, 733, 741, 34, 34, 34, 34, 34}; *///hallway 2

        /*deque<int> testScan = {34, 668, 33, 2, 33, 708, 33, 33, 33, 637, 33, 609, 33, 585, 33, 564, 33, 542, 33, 525, 33, 35, 34, 33, 33, 33, 482, 33, 518,
        33, 33, 33, 33, 33, 80, 53, 53, 327, 334, 334, 331, 329, 327, 326, 324, 34, 34, 34, 34, 34, 323, 322, 321, 320, 320, 317, 312, 53, 53, 53, 53, 2, 323,
        319, 320, 321, 321, 322, 34, 34, 34, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 380, 384, 389, 394, 399, 404, 412, 407, 421, 416,
        427, 438, 433, 2, 410, 408, 409, 53, 53, 371, 2, 53, 361, 53, 352, 347, 343, 337, 331, 329, 329, 2, 314, 322, 313, 311, 307, 305, 303, 300, 298, 296, 294,
        292, 291, 290, 288, 286, 285, 284, 283, 283, 281, 281, 280, 280, 279, 278, 279, 279, 282, 53, 53, 53, 37, 53, 275, 279, 280, 281, 282, 282, 284, 285, 286,
        287, 288, 289, 288, 286, 53, 53, 53, 2, 311, 309, 307, 310, 312, 315, 318, 322, 325, 328, 332, 336, 339, 344, 348, 352, 358, 362, 33, 372, 379, 385, 391, 397, 404, 411, 2, 2, 2, 437, 420, 2, 2, 2, 2, 418, 412, 408, 404, 399, 397, 391, 33, 383, 380, 377, 374, 372, 369, 367, 364, 362, 360, 358, 356, 355, 354, 352, 352, 350, 349, 348, 347, 346, 34, 34, 34, 34, 346, 347, 346, 347, 347, 347, 348, 349, 350, 351, 351, 352, 354, 356, 356, 358, 360, 362, 364, 365, 369, 371, 374, 376, 380, 385, 394, 53, 53, 53, 53, 396, 2, 2, 409, 2, 393, 390, 387, 380, 373, 374, 367, 365, 359, 355, 348, 340, 53, 53, 53, 346, 2, 333, 330, 328, 326, 324, 322, 321, 319, 318, 317, 315, 315, 314, 313, 323, 53, 53, 53, 655, 2, 648, 644, 640, 638, 636, 633, 631, 629, 626, 625, 624, 623,
        622, 621, 621, 620, 619, 620, 619, 621, 622, 622, 624, 625, 626, 627, 631, 631, 634, 637, 639, 641, 645, 647, 652, 656, 659, 665, 34, 34, 34, 34, 34,};
       */  //fake room 2
        
        void updateAngleSize();
        int findDistanceHallway(vector<int> averaged, int peak);
        int findDistanceDoor(deque<int> scan, vector<int> averaged, int peak);
        vector<int> getSubsection(deque<int> scan, int start, int end);
        bool isPeakHallway(vector<int> subsection);
        vector<int> findPeaks(vector<int> averaged, int start, int nd, bool checkEndpoints);
        vector<int> sortPeaks(deque<int> scan, vector<int> averaged, vector<int> peaks, bool isHallway);
        int getAngle(int peak, int rotation);
        DoublePoint getPoint(int angle, float distance);
        bool isSameAngle(float a1, float a2, float tolerance);
        std::vector<cluster> averageEstimations(std::vector<DH> dh);
        cluster shiftCluster(cluster c);
};

