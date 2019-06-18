$fn = 128;
with_frame=1;
with_akku=0;

with_props=0;
with_motor=1;
with_pcbs=0;
with_motor_brim=1;

name="phschoen";
version="v1.3";
text_depth=0.5;
with_cam=true;

motor_d = 8.75;//8.620;
motor_r=motor_d/2;
motor_height = 20;

motor_clamp_wall_thickness = 1.5;
motor_clamp_d= motor_d + 2*motor_clamp_wall_thickness;
motor_clamp_r= motor_clamp_d /2;
motor_clamp_cut_width = 3;
motor_clamp_extra_hight = 3;
motor_clamp_hight = motor_height + motor_clamp_extra_hight;

usb_width=12;
usb_hight=8;

motor_brim_d = 5;
motor_brim_h = 0.3;

motor_arm_width = 8;
motor_arm_height = 6;
motor_arm_length = 20;

body_width = 40;
body_height = 12;
body_wall_thickness = 2;
body_bottom_flange = 5;
body_bottom_bridge = 6;

cam_width = 14;
cam_angle = 20;
cam_pos = body_width/2 +5;
cam_hight = 5;

triangle_cut_height = 10;
cable_cut_width = 3;
cable_cut_height = 3;

akku_holder_hight = 9.5;
akku_holder_width =27;
motor_body_arm_dist = (body_width - motor_arm_width + 2) / sqrt(2);
eps=0.05;

use <lib/cube.scad>;
include <lib/triangle.scad>;

module triangle_cuts(h=4) {
    triangle_width= body_width / 2;
    h_tri=((body_width - body_bottom_flange - body_bottom_bridge)/2 - sqrt(2)* body_wall_thickness)* sqrt(2);
    for(r=[0:90:359])
    {
        rotate([0,0,r])
        translate([0,body_width/2 -body_bottom_flange,0]) {
            rotate([0,0,45])
                aligned_triangle(h_tri, h_tri, h);
        }
    }
}

module cam_tx06(with_hull = false) {
    q = 20;
    hcam = 13;         // camera height


    le = 0;
    translate([ 0,0, 0]) {
        color("green")
        if (with_hull)
        {
            hull()
            {
                translate([ 0,-7.0, 0]) cube([0.8,14,  hcam  ]);   // front pcb
                translate([-5,-7.0, 0]) cube([0.8,14,  hcam  ]);   // back pcb
                translate([-5,-2.5,13]) cube([0.8, 5,   2  ]);     // antena pcb
            }
        } else {
            translate([ 0,-7.0, 0]) cube([0.8,14,  hcam  ]);   // front pcb
            translate([-5,-7.0, 0]) cube([0.8,14,  hcam  ]);   // back pcb
            translate([-5,-2.5,13]) cube([0.8, 5,   2  ]);     // antena pcb
        }

        color("grey") {
            translate([0.1,0, 6]) rotate([0,90,0]) cylinder(d= 8, h=6,$fn=4*q);
            translate([6  ,0, 6]) rotate([0,90,0]) cylinder(d=10, h=3,$fn=4*q);
            translate([-6.25,4.,12]) cube([1, 2, 2]);
            translate([-4.5,5.,2]) cube([4.5, 2, 3.6]);
            translate([-4,0, 15]) rotate([0,le,0]) translate([0,0,  5]) cylinder(d=3, h=28, $fn=2*q);
            translate([-4,0, 15]) rotate([0,le,0]) translate([0,0, 10]) cylinder(d=5, h=10, $fn=3*q);
        }
        color("wheat") {
            translate([-4,0, 15]) rotate([0,le,0]) cylinder(d=1.5, h=5, $fn=2*q);
        }
        color("silver") {
            translate([-6.5,3,11]) cube([1.5, 4, 2]);
        }
    }
}

module pcb_flight_controller() {
    flight_controller = [20, 28,2];
    cube(flight_controller);
}

module pcb_rx_radio() {
    rx_radio=[12,25,2];
    cube(rx_radio);
}

module motor() {
    prop_height=motor_height + 5;
    translate([0, 0, -1])
    color([0.8,0.2,0.2,0.5]) {
        if (with_motor) {
            translate([0, 0, motor_clamp_extra_hight])
                %cylinder(d = motor_d, h = motor_height );
            translate([0, 0, motor_clamp_extra_hight])
                %cylinder(d = 2, h = prop_height);
        }
        if (with_props) {
            translate([0, 0, prop_height])
                %cylinder(d = 66.5, h = 2);
        }
    }
}

module motor_clamp() {
    // motor / prop mockup

    difference() {
        union () {
            color("blue")
            // motor stand
            hull() {
                cylinder(d = motor_d + 2*motor_clamp_wall_thickness, h = motor_clamp_hight);

                translate([0, -(motor_d + 2*motor_clamp_wall_thickness) / 2 - 2, 0])
                    aligned_cube([motor_arm_width, eps, motor_arm_height]);
            }

            // print support
            if(with_motor_brim) {
                color("green")
                cylinder(d = motor_clamp_d+motor_brim_d, h = motor_brim_h);
            }
        }

        // cable hole
        // motor hole
        translate([0, 0, -eps])
            cylinder(d = motor_d, h = motor_height + 2 +eps);
        // clamp cutout
        translate([0, motor_r, -eps])
            aligned_cube([motor_clamp_cut_width, 2 * motor_clamp_wall_thickness, motor_clamp_hight+2*eps]);
        // motor top hole
        translate([0, 0, motor_clamp_hight - motor_clamp_wall_thickness+eps])
            cylinder(d = 6.5, h = motor_clamp_wall_thickness);
        // angle motor top cut for better printing
        translate([0, 0, motor_clamp_hight-1-eps])
            cylinder(d1=motor_d, d2=6.5, h=0.75);
    }

    motor();
}

module motor_arm() {
    difference() {
        union() {
            translate([0, motor_arm_length+motor_d/2, 0])
                motor_clamp();
            color("blue") {
                aligned_cube([motor_arm_width, motor_arm_length, motor_arm_height],[1,0,0]);
            }
        }
        cable_cuts();
    }
}
module cable_cuts() {
        union() {
            // vertical cut
            translate([0, motor_arm_length, -eps])
                aligned_cube([cable_cut_width, cable_cut_height, cable_cut_height*2+2*eps]);

            // lower cut
            translate([0, motor_arm_length , -eps])
                aligned_cube([cable_cut_width, cable_cut_height, cable_cut_height+eps],[1,0,0]);

            // horizontal cut
            translate([0, -eps,motor_arm_height+eps])
                aligned_cube([cable_cut_width,motor_arm_length, , cable_cut_height],[1,0,2]);
        }
}

module body() {
    arm_offset=(body_width - motor_arm_width/sqrt(2))/sqrt(2);
    difference() {
        union() {
            // arms
            for(r = [45 : 90 : 360]) {
                rotate([0, 0, r])
                translate([0,arm_offset,0])
                    motor_arm();
            }

            // board mount
            color("red")
                aligned_cube([body_width, body_width, body_height]);

        }

        // board pcb cutout
        translate([0,0,body_wall_thickness])
            aligned_cube([body_width- 2*body_wall_thickness, body_width-2*body_wall_thickness, body_height]);

        // cable cutouts
        for(r = [45 : 90 : 360]) {
            rotate([0, 0, r])
            translate([0,arm_offset,0])
                cable_cuts();
        }
        // usb cutout
        translate([-2, -body_width / 2 - 1, 2])
            cube([usb_width, 4, usb_hight]);

        // base triangle cut-outs
        translate([0, 0, -1])
            triangle_cuts();


        // text stamp
        union()
        {
            if (with_cam == false)
            {
                rotate([0,0,90]) {
                    translate([0,-body_width/2+text_depth-eps,body_height/2])
                    rotate([90,0,0])
                    linear_extrude(height = text_depth)
                    {
                        translate([0,2,0])
                        text(halign="center",valign="center", $fn=$fn, size=5,font="Linux Libertine:style=Bold", "Toolbox");
                        translate([0,-3,0])
                        text(halign="center",valign="center", $fn=$fn, size=3,font="Linux Libertine:style=Bold", version);
                    }

                }
            }else {
                translate([body_width/2, 0, body_wall_thickness])
                aligned_cube([10, cam_width, body_height+eps]);
            }
            rotate([0,0,180]) {
                translate([0,-body_width/2+text_depth-eps,body_height/2])
                rotate([90,0,0])
                linear_extrude(height = text_depth)
                {
                    text(halign="center",valign="center", $fn=$fn, size=5,font="Linux Libertine:style=Bold", name);
                }
            }
        }

    }
            color("orange")
            if (with_cam) {
                difference() {
                    translate([body_width/2, 0, 0])
                    {
                        intersection() {
                        rotate([0,-cam_angle,0])
                        aligned_cube([10, cam_width+body_wall_thickness*2, body_height], [0,1,0]);

                        aligned_cube([10, cam_width+body_wall_thickness*2, body_height], [0,1,0]);
                        }

                    }

                    translate([body_width/2+5, 0, 0])
                    translate([3.5,0,5])
                    rotate([0,90-cam_angle,0])
                    rotate([0,0,90])
                    linear_extrude(height = text_depth)
                    text(halign="center",valign="center", $fn=$fn, size=3,font="Linux Libertine:style=Bold", version);

                    rotate([0,-cam_angle,0])
                    translate([cam_pos,0,-8.5])
                    scale([1.05,1.05,1.2])
                    {
                        for (i = [0:1:10])
                        translate([0,0,i + cam_hight])
                        cam_tx06(with_hull=true);
                    }
                }
            }
}

module quad() {
    if(with_frame) {
        body();
    }
    if(with_pcbs) {
        translate([0,0, body_wall_thickness+eps]) {
            translate([-3, -14, 0])
                %pcb_flight_controller();

            translate([-16, -12.5, 0])
                %pcb_rx_radio();
        }
    }
    if(with_akku) {
        translate([0,0,body_height]) {
            akku_holder();
        }
    }
    if (with_cam) {
        translate([cam_pos, 0, 0])
        rotate([0,-cam_angle,0])
        translate([0, 0, cam_hight])
            %cam_tx06();
    }
}

module akku_holder() {
    color([0.5,0.8,0.4,0.5])
    difference() {
        union() {
            aligned_cube([body_width,body_width,akku_holder_hight]);
            translate([0,0,-1])
                aligned_cube([body_width-body_wall_thickness*2,
                              body_width-body_wall_thickness*2,
                              akku_holder_hight]);
        }
        translate([0,0,1+0.01]) {
            aligned_cube([akku_holder_width,
                body_width+1,
                akku_holder_hight+eps]);
        }
        translate([0, 0, -2])
            triangle_cuts(h=6);
        translate([-body_width/2-10,0,0]) {
            aligned_cube([9,14,4],[-2,1,1]);
        }
    }
}


quad();
//body();
//motor_arm();
//motor_clamp();
//akku_holder();
