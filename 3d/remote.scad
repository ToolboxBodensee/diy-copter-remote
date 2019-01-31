use <lib/cube.scad>;
use <lib/cylinder.scad>;
use <lib/triangle.scad>;
use <lib/antenna.scad>;
use <stick.scad>;
use <lib/LCD_1602_I2C.scad>;
use <lib/PCB.scad>;
use <lib/Arduino_nano.scad>;
use <lib/threads.scad>;
$fn=64;

show_top            =1;
show_bottom         =1;
show_strapholder    =1;
show_joysticks      =1;
show_grip           =1;
show_electronics    =1;

enable_text_engrave =0;

show_switch         =1;
show_lcd            =1;
show_sticks         =1;
show_stm32          =1;
show_antenna        =1;
show_cc2500         =1;
show_batery_charger =1;
show_batery         =1;
show_joysticks_pcb  =1;

show_stands=0;

screw_d      = 2.98;
screw_d_loos = 3.25;
screw_head_d = 6.25;

top_bottom_screws=[
    [-44,-57,0],
    [ 44,-57,0],
    [-52, 53,0],
    [ 52, 53,0],
    [ 14.5, 30,0],
    [-14.5, 30,0],
];
grip_bottom_screws=[
    [-54,-45,0],
    [ 54,-45,0],
    [-54, 31,0],
    [ 54, 31,0],
    [-54, -7,0],
    [ 54, -7,0],
];


font="Go Mono:style=Bold";
font="Go Mono";
font="Arial Rounded MT Bold:style=Bold";
font_size=3.75;
font_size_name=6.5;
font_spaceing=1.25;


grip_h=14;

name="";

text_pos=[[-6.75,-17,0], [6.75,-17,0], [-43,55,0], [-20,55,0], [0,39,0],    [20,55,0], [43,55,0]   ];
top_text=["on",          "arm",        "mode",     "beeper",   "failsave",  "prearm",  "led"    ];
bot_text=["off",         "",           "",         "",         "",          "",        ""          ];

bottom_h=25;
pos_sticks=[40,26,0];
pos_cc2500=[0,12,-7];
pos_stm32=[40.5,-13.5,-4.5];
pos_antenna=[0,130/2+8.20,-15];
pos_lcd=[0,-43,-8.75];
pos_batery_charger=[-50,-40,-7];
rot_batery_charger=[0,0,90];
pos_batery=[-40,-10.25,-16];
rot_batery=[0,0,90];
pos_strap_holder=[0,10,3];
strap_screw_dist=13.5;

remote_top_plate_1=[140,110,0];
remote_top_plate_2=[130,110,0];
remote_top_plate_3=[100,136,0];
bottom_wall=3;

ps2_pcb_size=[26.15, 34.15, 1.6];
charger_pcb_size_small=[14.75, 26, 1.6];
charger_pcb_size=[18, 26.6, 1.6];
stm32_pcb_size=[52.3, 22.6, 1.6];
cc2500_pcb_size=[34.3, 21.4, 0.9];
batery_size=[24, 50, 15];

eps=0.1;
//$t=1;

module remote() {
    // animate sticks
    if($t < 0.25)
        sticks($t*4,0);
    else if($t < 0.5)
        sticks(1,($t-0.25)*4);
    else if($t < 0.75)
        sticks(1-($t-0.5)*4,1);
    else if($t < 1)
        sticks(0,1-($t-0.75)*4);
    else
        sticks(0.5,0.5);


    if(show_stm32) {
        translate(pos_stm32) {
            rotate([0,180,180])
            stm32_bluepill();
        }
    }

    if(show_antenna) {
        translate(pos_antenna)
        rotate([90,0,0]) {
            sma_connector_male(3.5);
            antenna();
        }
    }
    if(show_cc2500) {
        translate(pos_cc2500) {
            rotate([0,0,90])
            cc2500();
        }
    }
    if(show_lcd) {
        translate(pos_lcd)
        LCD_1602_I2C(3, "RSSI 99     A99 ", " ARMED      A99 ");
    }

    if(show_switch){
            for(i=[0:1:len(text_pos)-1]) {
                translate(text_pos[i])
                translate([0,0,-5])
                switch(thick);
            }
    }
    if(show_batery_charger) {
        translate(pos_batery_charger)
        rotate(rot_batery_charger)
        rotate([0,0,-90])
        batery_charger();
    }

    // batery box
    if(show_batery)
    {
        translate(pos_batery)
        rotate(rot_batery) {
            // side holder
            color("red")
                aligned_rounded_cube(batery_size,2,[1,1,1],[1,1,0]);
        }
    }


    // stands
    if(show_stands)
    {
        h=19.9;
        d=7;
        for(i=[0:1:len(top_bottom_screws)-1]) {
            translate(top_bottom_screws[i]-[0,0,h]) {
                difference() {
                    translate([0,0,0]) {
                        %cylinder($fn=6,d=d,  h=h);
                    }
                }
            }
        }
    }
    if(show_top)
        top_case();
    if(show_bottom)
        bottom_case();
    if(show_grip)
        left_right_grip();

    if(show_strapholder)
        strapholder();
}
module strapholder() {
    eps=0.1;
    translate(pos_strap_holder)
    {
        difference() {
            union() {
                translate([0,5,0])
                {
                    aligned_rounded_cube([2*(strap_screw_dist+5),15+15,2], r=2, rounding=[1,1,0]);
                    aligned_rounded_cube([2*(strap_screw_dist+5),15+15,6], r=2, rounding=[1,1,1]);
                }
                translate([0,0,5])
                hull() {
                    aligned_rounded_cube([20,15,eps], r=2, rounding=[1,1,0]);
                    aligned_rounded_cube([15,10,5], r=2, rounding=[1,1,1]);
                }
            }

            // name
            translate([0,13,5])
                text_engave(thick,font_size=font_size_name, text_="toolbox v1");

            // strap holder
            translate([0,0,12]) {
            rotate([0,90,0]) {
                rotate_extrude()
                    translate([8,0])
                            circle(d=4);
            }
            }
            // screw mount
            for (i=[-1,1]) {
                translate([i*strap_screw_dist,0,-eps])
                    cylinder(d=screw_d_loos, h=100);
                translate([i*strap_screw_dist,0,4-eps])
                    cylinder(d=screw_head_d, h=100);
            }
        }
    }
}

module sticks(l1,l2)
{
    max_angle=30;
    for(i=[1,-1]) {
        translate([pos_sticks[0]*i,pos_sticks[1],-10]) {
            if (show_joysticks) {
                color("orange")
                rotate([max_angle*(l1*2-1),max_angle*(l2*2-1),0])
                translate([0,0,7]) {
                    stick(h=20);
                }
            }
            if (show_joysticks_pcb) {
                translate([0,0,-8]) {
                    ps2_joystick(l1,l2);
                }
            }
        }
    }
}

module top_case() {
    %ps2_dust_wall();
    difference() {
        thick=4;
        color([0.5,0.8,0,0.8])
        union() {
            hull() {
                aligned_rounded_cube(remote_top_plate_1+[0,0,1],    7,[1,1,0],[1,1,0]);
                aligned_rounded_cube(remote_top_plate_2+[0,0,thick],2,[1,1,0],[1,1,0]);
                aligned_rounded_cube(remote_top_plate_3+[0,0,thick],2,[1,1,0],[1,1,0]);

            }
            // inner wall
            {
                b1=2*(bottom_wall+0.5);//bot_wall_width
                b2=2*(2);// top_innerwall width
                h1=2;    // top_innerwall hight
                translate([0,0,-h1+eps])
                difference()
                {
                    union() {
                        hull() {
                            aligned_rounded_cube(remote_top_plate_1+[-b1,-b1,h1],7,[1,1,0],[1,1,0]);
                            aligned_rounded_cube(remote_top_plate_2+[-b1,-b1,h1],2,[1,1,0],[1,1,0]);
                            aligned_rounded_cube(remote_top_plate_3+[-b1,-b1,h1],2,[1,1,0],[1,1,0]);
                        }
                    }
                    union() {
                        hull() {
                            aligned_rounded_cube(remote_top_plate_1+[-b1-b2,-b1-b2,h1*3],7,[1,1,0],[1,1,1]);
                            aligned_rounded_cube(remote_top_plate_2+[-b1-b2,-b1-b2,h1*3],2,[1,1,0],[1,1,1]);
                            aligned_rounded_cube(remote_top_plate_3+[-b1-b2,-b1-b2,h1*3],2,[1,1,0],[1,1,1]);
                        }
                    }
                }
            }
            // lcd screw holder
            translate([pos_lcd[0],pos_lcd[1]-1.2,0])
            translate([-40.6,-20.25,0])
            {
                d_top=8.75;
                d_bot=7.75;
                // screwsholders
                h=10;
                translate([0,1,thick-h]){
                    translate([2.5,4,-1]) rotate([0,0,0])
                    difference() {cylinder(d=d_bot,h=h); translate([0,0,-eps]) cylinder(d=screw_d, h=4);};
                    translate([2.5,35,-1]) rotate([0,0,0])
                    difference() {cylinder(d=d_top,h=h); translate([0,0,-eps]) cylinder(d=screw_d, h=4);};
                    translate([78.5,4,-1]) rotate([0,0,0])
                    difference() {cylinder(d=d_bot,h=h); translate([0,0,-eps]) cylinder(d=screw_d, h=4);};
                    translate([78.5,35,-1]) rotate([0,0,0])
                    difference() {cylinder(d=d_top,h=h); translate([0,0,-eps]) cylinder(d=screw_d, h=4);};
                }
            }

            // button holders
            {
                for(i=[0:1:len(text_pos)-1]) {
                    translate([0,0,thick-10])
                    translate(text_pos[i])
                    difference() {
                        aligned_cube([15,10,8]);
                        translate([0,0,-7.25])
                            aligned_cube([8.5,13,10]);
                    }
                }
            }

            // stm32 holder
            {
                translate(pos_stm32){
                    a=1.75;
                    w=8;
                    // side holder
                    stm32_pcb_size=[52.3, 22.6, 1.6];
                    for(y=[-1,1])
                    for(x=[-1,1]) {
                        translate([x*(stm32_pcb_size[0]/2-10),
                                   y*(stm32_pcb_size[1]/2+a),
                                   0.2]){
                            aligned_cube([w,6.25+a*2,stm32_pcb_size[2]-pos_stm32[2]],[1,1,0]);
                        }
                        translate([x*(stm32_pcb_size[0]/2-10),
                                   y*(stm32_pcb_size[1]/2+1.625+a),
                                   stm32_pcb_size[2]-2-2]){
                            aligned_cube([w,3.0+a*2,2.6],[1,1,0]);
                        }
                    }
                }
            }

            // cc2500 holder
            {
                translate(pos_cc2500){
                    a=2;
                    w=6;
                    for(y=[-1,1])
                    for(x=[-1,1]) {
                        translate([x*(12.75+a),y*8,1.25]){
                            aligned_cube([5+2*a,w,-pos_cc2500[2]],[1,1,0]);
                        }
                        translate([x*(13.2+a),y*8,1.25-2]){
                            aligned_cube([4.1+2*a,w,2],[1,1,0]);
                        }
                    }
                }
            }

            // batery charger holder
            {
                w=6;
                translate(pos_batery_charger)
                rotate(rot_batery_charger)
                {
                    for(x=[-1,1]) {
                        translate([0, x*(charger_pcb_size[0]/2+1),charger_pcb_size[2]+0.2]){
                            aligned_cube([w,6,-pos_batery_charger[2]],[1,1,0]);
                        }
                        translate([0, x*(charger_pcb_size[0]/2+2.1),0]){
                            aligned_cube([w,3.75,2-pos_batery_charger[2]],[1,1,0]);
                        }
                    }
                    translate([ -charger_pcb_size[0]/2-5,0,charger_pcb_size[2]+0.2]){
                        aligned_cube([5.5,w,-pos_batery_charger[2]],[1,1,0]);
                    }
                    translate([ -charger_pcb_size[0]/2-6.25,0,0]){
                        aligned_cube([3,w,2-pos_batery_charger[2]],[1,1,0]);
                    }
                }
            }

            // batery holder
            {
                translate(pos_batery-[0,0,0])
                rotate(rot_batery) {
                    wall=2.5;
                    difference() {
                        translate([0, 0,5])
                        aligned_rounded_cube(size=batery_size+[2*wall,-10,-wall-pos_batery[2]-batery_size[2]], r=2, rounding=[1,1,0]);
                        translate([0, -(eps+wall),0])
                        aligned_rounded_cube(size=batery_size, r=2, rounding=[1,1,0]);

                        for(i=[1,-1])
                        {
                            hull() {
                                for(j=[0.5,-0.5])
                                translate([0, j+i*batery_size[1]/4,8])
                                rotate([0,90,0])
                                cylinder (d=2, h=batery_size[0]*2,center=true);
                            }
                            hull() {
                                for(j=[0,-3])
                                translate([0, i*batery_size[1]/4,8+j])
                                rotate([0,90,0])
                                cylinder (d=2, h=batery_size[0]*2,center=true);
                            }
                        }
                    }
                }
                // ps2 dust wall
            }

            // stap screw cutouts
            translate(pos_strap_holder) {
                // screw mount
                for (i=[-1,1]) {
                    translate([i*strap_screw_dist,0,-6])
                    difference(){
                            aligned_rounded_cube([10,10,4],1,[1,1,1]);
                        w=6;
                        cylinder($fn=6, r=w / 2 / cos(180 / 6) + 0.05, h=20, center=true);
                    }
                }
            }

        }//end of shape
        // begin of cutouts
        {
            // screw place
            for(i=[0:1:len(top_bottom_screws)-1]) {
                h=2;
                translate(top_bottom_screws[i]-[0,0,-thick+h-eps]) {
                    cylinder(d=screw_head_d, h=h);
                }
            }
            for(i=[0:1:len(top_bottom_screws)-1]) {
                h=30;
                translate(top_bottom_screws[i]-[0,0,-thick+h-eps]) {
                    cylinder(d=screw_d_loos, h=h);
                }
            }
        }

        //lcdcutout
        translate([pos_lcd[0],pos_lcd[1],-eps]) {
            color("green") {
                aligned_rounded_cube(size=[64,14,5+2*eps], r=2, rounding=[1,1,0]);
                hull() {
                    e=10;
                    translate([0,0,0.6])
                        aligned_rounded_cube(size=[65,15,eps], r=2, rounding=[1,1,0]);
                    translate([0,0,0.6+thick])
                        aligned_rounded_cube(size=[65+e,15+e,eps], r=2, rounding=[1,1,0]);
                }
            }
        }

        // stap screw cutouts
        translate(pos_strap_holder) {
            // screw mount
            for (i=[-1,1]) {
                translate([i*strap_screw_dist,0,-7-eps])
                    cylinder(d=screw_d_loos, h=100);
            }
        }

        // stick cutouts
        color("orange")
        for(i=[1,-1]) {
            w=32;
            r=14;
            translate([pos_sticks[0]*i,pos_sticks[1],-eps]) {
                aligned_rounded_cube(size=[w,w,thick+2*eps], r=r, rounding=[1,1,0]);
                hull() {
                    e=5;
                    translate([0,0,thick])
                        aligned_rounded_cube(size=[w,w,eps], r=r, rounding=[1,1,0]);
                    translate([0,0,0])
                        aligned_rounded_cube(size=[w+e,w+e,eps], r=r, rounding=[1,1,0]);
                }
                hull() {
                    e=2;
                    translate([0,0,thick+0.3])
                        aligned_rounded_cube(size=[w+e,w+e,eps], r=r, rounding=[1,1,0]);
                    translate([0,0,thick-1])
                        aligned_rounded_cube(size=[w,w,eps], r=r, rounding=[1,1,0]);
                }
            }
        }

        // switches cutouts and naming
        {
            translate([0,pos_lcd[1]-17,eps])
                text_engave(thick, font_size=font_size_name, text_=name);

            // switches
            for(i=[0:1:len(text_pos)-1]) {
                translate(text_pos[i]) {
                    switch_with_text(thick=thick+2*eps,
                                top_text=top_text[i],
                                bottom_text=bot_text[i]);
            }
        }
        }

    }
}

module switch_with_text(thick=4,top_text="on",bottom_text="off")
{
    dist_text=9.5;
    cylinder(d=6.35, h=5*thick+2*eps, center=true);


    color("gray")
    translate([0,0,thick-1.5-0.19])
    union() {
        cylinder(d=12.5, h=1.5);
        translate([0,-2.25,0])
        aligned_cube([2.75,12.5,1.5],[1,1,0]);
    }

    if(enable_text_engrave) {
    translate([0,dist_text,0])
            text_engave(thick, text_=top_text);
        translate([0,1-dist_text-2.5,0])
            text_engave(thick,  text_=bottom_text);
    }
}

module text_engave(thick, text_)
{
    h=1;
    translate([0,0,thick-h]) {
        linear_extrude(height = h) {
            text(
                    halign="center",
                    valign="center",
                    $fn=$fn,
                    size=font_size,
                    font=font,
                    spacing=font_spaceing,
                    text_
            );
        }
    }
}

module bottom_case() {
    difference() {
        color([1,1,1,0.8])
        union() {
            //body itself
            translate([0,0,-bottom_h])
            difference() {
                union() {
                    // top hull
                    hull() {
                        aligned_rounded_cube(remote_top_plate_1+[0,0,bottom_h],7,[1,1,0],[1,1,0]);
                        aligned_rounded_cube(remote_top_plate_2+[0,0,bottom_h],2,[1,1,0],[1,1,0]);
                        // middle plate extended a bit lower
                        aligned_rounded_cube(remote_top_plate_3+[0,0,bottom_h],2,[1,1,0],[1,1,0]);
                    }
                }
                // inner cutout should be flat so move all down
                color([0.8,0.8,0.8,0.8])
                translate([0,0,bottom_wall+1])
                translate([0,0,+1])
                hull() {
                    s=[2*bottom_wall,2*bottom_wall,0];
                    aligned_rounded_cube(remote_top_plate_1 +[0,0,bottom_h] - s,7,[1,1,0],[1,1,0]);
                    aligned_rounded_cube(remote_top_plate_2 +[0,0,bottom_h] - s,2,[1,1,0],[1,1,0]);
                    aligned_rounded_cube(remote_top_plate_3 +[0,0,bottom_h]- s,2,[1,1,0],[1,1,0]);

                }
            }

            // ps2 dust wall
            ps2_dust_wall();
            // ps2 holder
            for(i=[1,-1]) {
                h=6;
                translate([0,0,-h])
                translate([pos_sticks[0]*i,pos_sticks[1],-10]) {
                    translate([0,0,-8]) {
                        color("yellow")
                        for(x=[1,-1]) {
                                // screwholes near pin
                                translate([x*(ps2_pcb_size[0]/2-2.7), ps2_pcb_size[1]/2-4.5,-eps])
                                    cylinder(d=10,h=h);
                                // screwholes other side
                                translate([x*(ps2_pcb_size[0]/2-2.7), 2.9-ps2_pcb_size[1]/2,-eps])
                                    cylinder(d=10,h=h);
                                }
                        }
                }
            }
            // grip female screw holder
            for(i=[1,-1]) {
                difference() {
                    for(i=[0:1:len(grip_bottom_screws)-1]) {
                        translate([0,0,bottom_wall-bottom_h])
                        translate(grip_bottom_screws[i]) {
                            aligned_rounded_cube([10,10,1.7],2,[1,1,0],[1,1,-2]);
                        }
                    }
                }
            }
        }
        // ps2 holder
        for(i=[1,-1]) {
            h=6;
            h_screw=6;
            translate([0,0,-h])
            translate([pos_sticks[0]*i,pos_sticks[1],-10]) {
                translate([0,0,-8]) {
                    color("yellow")
                    for(x=[1,-1]) {
                            // screwholes near pin
                            translate([x*(ps2_pcb_size[0]/2-2.7), ps2_pcb_size[1]/2-4.5,-eps])
                            translate([0,0,h-h_screw])
                                cylinder(d=screw_d,h=h_screw+eps);
                            // screwholes other side
                            translate([x*(ps2_pcb_size[0]/2-2.7), 2.9-ps2_pcb_size[1]/2,-eps])
                            translate([0,0,h-h_screw])
                                cylinder(d=screw_d,h=h_screw+eps);
                    }
                }
            }
        }

        {
            // sma connector cutout
            color("gold") {
                translate(pos_antenna)
                rotate([90,0,0]) {
                    translate([0,0,10]) // inner sma offset
                    cylinder(d=6.5,h=10, center=true);
                }
            }
            // usb connector cutout
            color("red") {
                w=1.5;
                translate(pos_stm32)
                translate([stm32_pcb_size[0]/2+4+w, 0,-1.75-5]) // undo moveing
                {
                    aligned_rounded_cube([8,12,8], 3, [0,1,1]);
                    translate([-w-eps, 0,4]) // undo moveing
                        aligned_cube([8,7,3], [1,1,1]);
                }
            }
        }

        //bot_screw holes
        {
            top_bottom_case_srews();
        }

        // grip holes for screws
        left_right_grip_srews();
    }


}
module switch() {
    // screw
    color("silver")
    cylinder(d=6.4,h=8.75);

    // stick
    color("silver")
    translate([0,0,8.75])
    rotate([24,0,0]) {
        sphere(d=3);
        cylinder(d=3,h=8.5);
        translate([0,0,8.5])
            sphere(d=3);
    }

    // body
    color([0.3,0.3,0.6,1])
        aligned_cube([8.05,13,10],[1,1,2]);

    // pins
    color("silver")
    for(i=[1,0,-1]) {
        translate([0, i*4.5,-10])
        difference() {
            aligned_cube([2,0.7, 4.2], [1,1,2]);
            translate([0,0,-3])
            rotate([90,0,0])
                cylinder(d=1, h=2, center=true);
        }
    }

    // top plate
    color("silver")
    translate([0,0,7])
    difference() {
        union() {
            cylinder(d=12, h=0.7);
            aligned_cube([2.6, 8, 0.7],[0,1,0]);
        }
        translate([0,0,-0.1])
            cylinder(d=6.1, h=2);
    }
    // top screw
    color("silver")
    translate([0,0,7.7])
    difference() {
        cylinder($fn=6, d=9.4, h=0.7);
        translate([0,0,-0.1])
            cylinder(d=6.1, h=2);
    }
}
module stm32_bluepill() {
    eps=0.1;
    pitch=2.54;

    pin_offset=[ [2.35,3.65], [2.35, 18.6]];

    pin_names=[ [
        "G", "G", "3.3", "R", "B11", "B10", "B1", "B0", "A7", "A6",
        "A5", "A4", "A3", "A2", "A1", "A0", "C15", "C14", "C13", "VB",
    ], [
        "B12", "B13", "B14", "B15", "A8", "A9", "A10", "A11", "A12",
        "A15", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "5V", "G", "3.3",
    ]];

    difference() {
        union() {
            //pcb
            color("LightBlue") {
                aligned_cube(stm32_pcb_size);
            }
            // IC
            color("black")
            rotate([0,0,45])
            translate([0,0,stm32_pcb_size[2]])
            aligned_cube([7,7,2],[1,1,0]);

            // IC pins
            color("silver")
            rotate([0,0,45])
            translate([0,0,stm32_pcb_size[2]])
            aligned_cube([8,8,1],[1,1,0]);

            translate([stm32_pcb_size[0]/2,0 ,stm32_pcb_size[2]])
            usb_micro();

            // pins pads
            for(p=[0,1])
            for(i=[0:1:len(pin_names[p])-1]) {
                translate([-stm32_pcb_size[0]/2+pin_offset[p][0]+i*pitch,
                           -stm32_pcb_size[1]/2+pin_offset[p][1],-eps])
                {
                    color("silver") {
                        cylinder(d=1.5,h=stm32_pcb_size[2]+2*eps);
                    }
                    color("black") {
                        translate([0,(p-0.5)*2*2,stm32_pcb_size[2]+eps])
                        linear_extrude(height = eps)
                        text(
                                halign="center",
                                valign="center",
                                $fn=$fn,
                                size=1,
                                font=font,
                                pin_names[p][i]
                        );
                    }
                }
            }
        }
        // pin drill hole
        for(p=[0,1])
        for(i=[0:1:len(pin_names[p])-1]) {
            translate([-stm32_pcb_size[0]/2+pin_offset[p][0]+i*pitch,
                       -stm32_pcb_size[1]/2+pin_offset[p][1],-2*eps])
            color("Silver") {
                cylinder(d=0.7,h=stm32_pcb_size[2]+4*eps);
            }
        }

    }
}

module usb_micro() {

    color("Silver")
    difference() {
        size= [7.45, 6, 2.5];
        aligned_cube(size,[2,1,0]);

    }
}
module cc2500() {
    eps=0.1;
    pitch=2;

    pin_offset=[0.25,2];

    pin_names=[
        "LNA_EN",
        "PA_EN",
        "CSn",
        "GDO0",
        "GND",
        "GDO2",
        "SO",
        "SCLK",
        "SI",
        "VCC"
    ];

    difference() {
        union() {
            //pcb
            color("LightBlue") {
                aligned_cube(cc2500_pcb_size);
            }
            // hf cage
            color("silver")
            translate([0,0,cc2500_pcb_size[2]])
            aligned_cube([26.84,19.75,3.2],[1,1,0]);

            // pins pads
            for(i=[0:1:len(pin_names)-1]) {
                translate([-cc2500_pcb_size[0]/2+pin_offset[0],
                           -cc2500_pcb_size[1]/2+pin_offset[1]+i*pitch,-eps])
                {
                    color("silver") {
                        cylinder(d=1.5,h=cc2500_pcb_size[2]+2*eps);
                    }
                    color("black") {
                        rotate([180,0])
                        translate([1,0,-eps])
                        linear_extrude(height = eps)
                        text(
                                halign="left",
                                valign="center",
                                $fn=$fn,
                                size=1,
                                font=font,
                                pin_names[i]
                        );
                    }
                }
            }
        }
        // pin drill hole
        for(p=[0,1])
        for(i=[0:1:len(pin_names)-1]) {
            translate([-cc2500_pcb_size[0]/2+pin_offset[0],
                       -cc2500_pcb_size[1]/2+pin_offset[1]+i*pitch,-2*eps])
            color("Silver") {
                cylinder(d=0.7,h=cc2500_pcb_size[2]+4*eps);
            }
        }

    }
}
module batery_charger() {
    eps=0.1;

    // pcb
    color("lightblue")
    aligned_cube(charger_pcb_size);

    // Pins charge
    color("silver") {

        mirror([1,0,0]) {
            translate(charger_pcb_size/2)
            aligned_cube([2.38, 4, charger_pcb_size[2]+eps], [2,2,1]);
        }
        translate(charger_pcb_size/2)
        aligned_cube([2.38, 4, charger_pcb_size[2]+eps], [2,2,1]);
    }

    // Pins bat
    color("silver") {

            mirror([1,0,0]) {
                translate(charger_pcb_size/2 + [-1,-17,0])
                cylinder(d=1.7,h=charger_pcb_size[2]+eps, center=true);
                translate(charger_pcb_size/2 + [-1,-19.5,0])
                cylinder(d=1.7,h=charger_pcb_size[2]+eps, center=true);
            }

            translate(charger_pcb_size/2 + [-1,-17,0])
            cylinder(d=1.7,h=charger_pcb_size[2]+eps, center=true);


            translate(charger_pcb_size/2 + [-1,-19.5,0])
            cylinder(d=1.7,h=charger_pcb_size[2]+eps, center=true);
    }

    translate([0, charger_pcb_size[1]/2-7,charger_pcb_size[2]])
    rotate([0,0,-90])
    usb_micro();

}

module ps2_joystick(l1,l2)
{
    eps=0.1;
    max_angle=30;

    // joystick module
    /* color("silver") */
    /* translate([0,8.75]) */
    // jostick module iself
    color("silver")
    translate(ps2_pcb_size/2)
    translate([-4.1,-9,0]) {
        aligned_cube([16,16, 12.5], [2,2,0]);
        translate([-8,-8,3.5])
        rotate([max_angle*(l1*2-1),max_angle*(l2*2-1),0])
        translate([0,0,9])
        cylinder(d=4, h=7);
    }
    // potties
    color("darkgray")
    translate(ps2_pcb_size/2)
    translate([-0.5,-12.4,0]) {
        aligned_cube([3.8, 9.52, 12], [2,2,0]);
    }
    color("darkgray")
    translate(ps2_pcb_size/2)
    translate([-7.35,-25,0]) {
        aligned_cube([9.52, 3.8,12], [2,2,0]);
    }

            /* color("silver") { */
            /*     rotate([90,0]) */
            /*     import("ps2_analog.stl"); */
            /* } */

    difference()
    {
        pin_x=[2,1,0,-1,-2];
        pin_names=["GND", "+5V", "VRX", "VRY", "SW"];
        union()
        {
            // pcb
            color("lightblue")
            aligned_rounded_cube(ps2_pcb_size, r=1, rounding=[1,1,0]);

            //pinpads
            for(i=[0:len(pin_x)-1]) {
                translate([pin_x[i]*(2), ps2_pcb_size[1]/2-4.5,-eps]) {
                    color("silver")
                    cylinder(d=1.5,h=ps2_pcb_size[2]+2*eps);

                    color("black")
                    translate([0,-1,ps2_pcb_size[2]])
                    linear_extrude(height = 2*eps)
                    rotate([0,0,-90])
                    text(
                            halign="left",
                            valign="center",
                            $fn=$fn,
                            size=1,
                            font=font,
                            pin_names[i]
                    );
                }
            }

        }

        // pins
        for(i=[0:len(pin_x)-1]) {
            translate([pin_x[i]*(2), ps2_pcb_size[1]/2-4.5,-2*eps]) {
                cylinder(d=1,h=ps2_pcb_size[2]+4*eps);
            }
        }
        // screwholes near pin
        for(x=[1,-1]) {
            translate([x*(ps2_pcb_size[0]/2-2.7), ps2_pcb_size[1]/2-4.5,-eps])
            cylinder(d=2.9,h=ps2_pcb_size[2]+2*eps);
        }
        // screwholes other side
        for(x=[1,-1]) {
            translate([x*(ps2_pcb_size[0]/2-2.7), 2.9-ps2_pcb_size[1]/2,-eps])
            cylinder(d=2.9,h=ps2_pcb_size[2]+2*eps);
        }

    }

}
module ps2_dust_wall() {
    for(i=[1,-1]) {
        w=40;
        b=37;
        h=24;
        r=5;
        wall_thickness=2.5;
        translate([pos_sticks[0]*i+i*0,pos_sticks[1],-0.5-h-eps]) {
            difference() {
                aligned_rounded_cube(size=[b+wall_thickness,w+wall_thickness,h], r=r, rounding=[1,1,0]);

                translate([0,0,+eps])
                aligned_rounded_cube(size=[b,w,h+45], r=r, rounding=[1,1,0]);

                // cable hole
                translate([1*i,w/2,5])
                aligned_rounded_cube([10,10,3], r=1, rounding=[1,0,1]);

            }
        }
    }
}

module left_right_grip() {
    // left and right grip
    for(i=[1,-1]) {
        difference() {
            translate ([-i*2.5,0,-bottom_h])
            hull() {
                union() {
                    translate([i*(remote_top_plate_1[1]/2),0,-grip_h]) {
                        aligned_rounded_cube([bottom_h,100,grip_h],6 );
                    }
                }
                translate([i*remote_top_plate_1[1]/2-i*5,0,0]) {
                    aligned_rounded_cube([bottom_h+20,109,0.1],6,[1,1,0]);
                }
            }

            left_right_grip_srews();
        }
    }
}
module left_right_grip_srews(l=15,head=50, h=-21+3) {
    w=6;
    eps=0.1;
    // left and right grip
    translate ([0,0,h])
    for(i=[1,-1]) {
        difference() {
            for(i=[0:1:len(grip_bottom_screws)-1]) {
                translate(grip_bottom_screws[i]) {
                    color("darkgray")
                    translate([0,0,-l]) {
                        cylinder(d=screw_d_loos,  h=l+2*eps);
                    }
                    color("darkgray")
                    translate([0,0,eps-l-head]) {
                        cylinder(d=screw_head_d,  h=head);
                    }
                    color("lightgray")
                    translate([0,0,-4]) {
                    cylinder($fn=6, r=w / 2 / cos(180 / 6) + 0.05, h=4+eps);
                    }
                }
            }
        }
    }
}
module top_bottom_case_srews(l=26+2.5,head=50, h=2.1) {
    w=6;
    eps=0.1;
    translate([0,0,h])
    for(i=[0:1:len(top_bottom_screws)-1]) {
        th=2.5;
        h_screw_head=10;
        h_female_screw=20;
        //head
        translate(top_bottom_screws[i])
            cylinder(d=screw_head_d,  h=h+1);
        // thread
        translate(top_bottom_screws[i] + [0,0,-l+eps])
            cylinder(d=screw_d_loos,  h=l);

        translate(top_bottom_screws[i] + [0,0, -l+eps-h_female_screw+4])
            //cylinder(d=screw_head_d,  h=h_screw2);
            cylinder($fn=6, r=w / 2 / cos(180 / 6) + 0.05, h=h_female_screw);
    }
}
remote();
///left_right_grip_srews();
//bottom_case();
//top_bottom_case_srews();


