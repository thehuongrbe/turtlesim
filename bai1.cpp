#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

#include <iostream>
#include <algorithm>
using namespace std;

ros::Publisher pub;
const float PI = 3.14159265;
float rate = 5;
turtlesim::Pose current_pose;
bool back=false;

geometry_msgs::Twist getMessage(double linear_x, double angular_z)
{
    geometry_msgs::Twist msg;
    msg.linear.x = linear_x;
    msg.angular.z = angular_z;
    return msg;
}

void poseCallback(const turtlesim::Pose::ConstPtr& msg)
{
    cout << msg->x << " " << msg->y << endl;
    current_pose = *msg;
}
int main(int argc, char** argv)
{


    ros::init(argc, argv, "myturtle_control");
    ros::NodeHandle h;
    pub = h.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
    ros::Subscriber sub =
        h.subscribe("/turtle1/pose", 1000, poseCallback);
    ros::Rate loopRate(rate);

    double x0, y0;
    const double tolerance = 1e-2;
 bool test=true;//dieu khien tien lui
       for(int i=0;i<argc/2;i++){
             x0=atof(argv[2*i+1]);
            y0=atof(argv[2*i+2]);
        while (ros::ok()) {
            loopRate.sleep();
            ros::spinOnce();
            double distance = sqrt( pow(x0-current_pose.x, 2) + pow(y0-current_pose.y, 2) );
            if (distance < tolerance) { // dừng robot khi đến đủ gần
                //pub.publish(getMessage(0,0));
		        test=true;
                break;
            }
            double alpha = atan2( y0-current_pose.y, x0-current_pose.x ),a_z;
        
            double dx = x0 - current_pose.x, dy = y0 - current_pose.y, theta = current_pose.theta;

            double dalpha = asin ((cos(theta)*dy-sin(theta)*dx) / distance);

	//cout<<"goc: "<<alpha<<" "<<current_pose.theta<<" "<<dalpha<<endl;
        if(alpha>=current_pose.theta+PI-1.5&&alpha<=current_pose.theta+PI+1.5&&test==true){	
		 geometry_msgs::Twist msg = getMessage(
                -min(4*distance,8.0),-4*dalpha);
	pub.publish(msg);

	}
                /* code */
else{
            geometry_msgs::Twist msg = getMessage(
                min(4*distance,6.0),
                6*dalpha);
		test=false;
pub.publish(msg);
}         
        }
    }
    return 0;
}
