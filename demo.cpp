#include <iostream>
//#include "entt-master/src/entt/entt.hpp"
//#include "Catch2-devel/src/catch2/catch_test_macros.hpp"

using namespace std;

//all units are assumed to be meters, seconds, or a combination of the two

#define shooter_x 80.f
#define shooter_y 0.f
#define shooter_z 0.f

#define target_x 0.f
#define target_y 0.f
#define target_z 0.f


#define velocity 40.f
#define mass 1.f
#define step (float)1

#define CATCH_CONFIG_MAIN

float* calc_initial(entt::registry &registry, entt::entity &shooter, entt::entity &target, float* angles);
bool check_distance(float x, float z, float v);

// components
struct position{
    float x;
    float y;
    float z;
};

 struct movement{
    float bullet_velocity;
    float bullet_mass;
    float step_time;
    float direction_x;
    float direction_y;
    float direction_z;
    float gravity;
};
// components

// systems
void move(entt::registry &registry, entt::entity &bullet, entt::entity &target ){
    float from_x, from_y, from_z, to_x, to_y, to_z;
    from_x = registry.get<position>(bullet).x;
    from_y = registry.get<position>(bullet).y;
    from_z = registry.get<position>(bullet).z;

    registry.get<position>(bullet).x += registry.get<movement>(bullet).direction_x * registry.get<movement>(bullet).bullet_velocity * registry.get<movement>(bullet).step_time;
    registry.get<position>(bullet).y += registry.get<movement>(bullet).direction_y * registry.get<movement>(bullet).bullet_velocity * registry.get<movement>(bullet).step_time;
    registry.get<position>(bullet).z += registry.get<movement>(bullet).direction_z * registry.get<movement>(bullet).bullet_velocity * registry.get<movement>(bullet).step_time;
    registry.get<position>(bullet).z -= (registry.get<movement>(bullet).gravity + 9.81 * registry.get<movement>(bullet).step_time / 2);
    registry.get<movement>(bullet).gravity += 9.81 * registry.get<movement>(bullet).step_time;

    to_x = registry.get<position>(bullet).x;
    to_y = registry.get<position>(bullet).y;
    to_z = registry.get<position>(bullet).z;

    if(from_z > registry.get<position>(target).z && to_z < registry.get<position>(target).z){
        cout<<"HIT \"at\"";
        cout<<" ["<<to_x<<" , "<<to_y<<" , "<<to_z<<"]\n";
    }
    else{
        cout<<"Bullet moved from ["<<from_x<<" , "<<from_y<<" , "<<from_z<<"]";
        cout<<" to ["<<to_x<<" , "<<to_y<<" , "<<to_z<<"]\n";
    }
} 
// systems

int main(){
    entt::registry registry;

    auto bullet = registry.create();
    registry.emplace<position>(bullet, shooter_x, shooter_y, shooter_z);
    registry.emplace<movement>(bullet, velocity, mass, step, 0.f, 0.f, 0.f);

    auto target = registry.create();
    registry.emplace<position>(target, target_x, target_y, target_z);

    if(registry.get<movement>(bullet).bullet_velocity == 0){
        cout<<"The bullet can not reach the target";
            return 0;
    }

    float* angle;
    angle = calc_initial(registry, bullet, target, angle);

    if(angle[0] == 0.f && angle[1] == 0.f){
        if(registry.get<position>(bullet).x != registry.get<position>(target).x){
            cout<<"The bullet can not reach the target";
            return 0;
        }
        if(registry.get<position>(bullet).y != registry.get<position>(target).y){
            cout<<"The bullet can not reach the target";
            return 0;
        }
        if(registry.get<position>(bullet).z != registry.get<position>(target).z){
            cout<<"The bullet can not reach the target";
            return 0;
        }
    }

    registry.get<movement>(bullet).direction_x = round(cosf(angle[0])*1000)/1000*round(cosf(angle[1])*1000)/1000;
    registry.get<movement>(bullet).direction_y = round(sinf(angle[0])*1000)/1000*round(cosf(angle[1])*1000)/1000;
    registry.get<movement>(bullet).direction_z = round(sinf(angle[1])*1000)/1000;
    
    cout<<"directional vector ["<< registry.get<movement>(bullet).direction_x <<" , "
    << registry.get<movement>(bullet).direction_y <<" , "<< registry.get<movement>(bullet).direction_z <<"]\n";


    cout<<"angle with x axis horizontally:"<<angle[0]*180/(atan(1)*4)<<" degrees\n";
    cout<<"angle with x axis vertically:"<<angle[1]*180/(atan(1)*4)<<" degrees\n";

/* simulation of shooting - tracking the movement of a projectile
    float before_step, after_step;
    
    do{
        before_step = registry.get<position>(bullet).z;
        move(registry, bullet, target);
        after_step = registry.get<position>(bullet).z;
    }while(!(before_step >= registry.get<position>(target).z && after_step < registry.get<position>(target).z));
*/
    return 0;
}

float* calc_initial(entt::registry &registry, entt::entity &shooter, entt::entity &target, float* angles){
    
    float quadrant;

    float x0 = registry.get<position>(shooter).x;
    float y0 = registry.get<position>(shooter).y;
    float z0 = registry.get<position>(shooter).z;

    float x1 = registry.get<position>(target).x;
    float y1 = registry.get<position>(target).y;
    float z1 = registry.get<position>(target).z;

    float v0 = registry.get<movement>(shooter).bullet_velocity;

    float z = z0 = z0-z1;
    angles = (float*)malloc(2*sizeof(float));

    if(x0<x1 && y0<y1){
        quadrant = 0;
    }
    if(x0>=x1 && y0<y1){
        quadrant = 1;
    }
    if(x0>=x1 && y0>=y1){
        quadrant = 2;
    }
    if(x0<x1 && y0>y1){
        quadrant = 3;
    }

    if(x0*x1 >= 0){
        x0 = abs(abs(x0)-abs(x1));
    }
    else{
        x0 = abs(x0)+abs(x1);
    }
    
    if(y0*y1 >= 0){
        y0 = abs(abs(y0)-abs(y1));
    }
    else{
        y0 = abs(y0)+abs(y1);
    }

    float x = sqrt(x0*x0 + y0*y0);

    if(!check_distance(x, z, v0)){
        angles[0] = 0.f;
        angles[1] = 0.f;
        return angles;
    }

    float angle_z = x * x * 9.81;
    float tmp = v0*v0;
    angle_z /= tmp;
    angle_z -= z;
    tmp = x * x + z * z;
    tmp = sqrt(tmp);
    angle_z /= tmp;
    tmp = x/z;
    angle_z = acosf(angle_z);
    tmp = atanf(tmp);
    angle_z = (angle_z + tmp) / 2;

    float angle_x = acosf(x0/x)+quadrant*(atan(1)*2);

    angles[0] = angle_x;
    angles[1] = angle_z;

    return angles;
}

bool check_distance(float x, float z, float v){
    float max_distance, time_up, time_down, max_height;

    time_up = sinf(atan(1)) * v/9.81;
    max_height = v*v*(sinf(atan(1)))*(sinf(atan(1)))/(2*9.81);
    time_down = sqrt(2*(max_height+z)/9.81);
    max_distance = v*(cosf(atan(1)))*(time_up+time_down);

    if(x<max_distance)
        return true;

    return false;
}

// Tests are inactive due issues with the catch2 library
/*TEST_CASE( "Testing angles", "[factorial]" ) {
    entt::registry testing_registry;

    // pair 1 - test x axis
    auto bullet_1 = registry.create();
    registry.emplace<position>(bullet_1, 0.f, 0.f, 0.f);
    registry.emplace<movement>(bullet_1, 20, mass, step, 0.f, 0.f, 0.f);

    auto target_1 = registry.create();
    registry.emplace<position>(target_1, 0.f, 40, 0.f);

    // pair 2 test y axis
    auto bullet_2 = registry.create();
    registry.emplace<position>(bullet_2, 0.f, 0.f, 10.f);
    registry.emplace<movement>(bullet_2, 20, mass, step, 0.f, 0.f, 0.f);

    auto target_2 = registry.create();
    registry.emplace<position>(target_2, 40, 0.f, 0.f);

    // pair 3 test shooting from negtives
    auto bullet_3 = registry.create();
    registry.emplace<position>(bullet_3, -32, -24, 0.f);
    registry.emplace<movement>(bullet_3, 20, mass, step, 0.f, 0.f, 0.f);

    auto target_3 = registry.create();
    registry.emplace<position>(target_3, 0.f, 0.f, 0.f);

    // pair 4 test out of range
    auto bullet_4 = registry.create();
    registry.emplace<position>(bullet_4, 0.f, 0.f, 0.f);
    registry.emplace<movement>(bullet_4, 40, mass, step, 0.f, 0.f, 0.f);

    auto target_4 = registry.create();
    registry.emplace<position>(target_4, 200, target_y, target_z);

    // pair 5 test angles 0,0
    auto bullet_5 = registry.create();
    registry.emplace<position>(bullet_5, 0.f, 0.f, 0.f);
    registry.emplace<movement>(bullet_5, 20, mass, step, 0.f, 0.f, 0.f);

    auto target_5 = registry.create();
    registry.emplace<position>(target_5, 57.2, target_y, -40.f);

    float* results;

    REQUIRE( round(calc_initial(registry, bullet_1, target_1, results)[0]) == 0 );
    REQUIRE( round(calc_initial(registry, bullet_1, target_1, results)[1]) == 39 );
    
    REQUIRE( round(calc_initial(registry, bullet_2, target_2, results)[0]) == 90 );
    REQUIRE( round(calc_initial( registry, bullet_2, target_2, results)[1]) == 60.44 );
    
    REQUIRE( round(calc_initial( registry, bullet_3, target_3, results)[0]) == 36.87 );
    REQUIRE( round(calc_initial( registry, bullet_3, target_3, results)[1]) == 60.44 );
    
    REQUIRE( round(calc_initial( registry, bullet_4, target_4, results)[0]) == 0 );
    REQUIRE( round(calc_initial( registry, bullet_4, target_4, results)[1]) == 0 );

    REQUIRE( round(calc_initial( registry, bullet_5, target_5, results)[0]) == 0 );
    REQUIRE( round(calc_initial( registry, bullet_5, target_5, results)[1]) == 0 );
}*/

