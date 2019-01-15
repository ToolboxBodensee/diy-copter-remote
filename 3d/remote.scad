use <lib/cube.scad>;
use <lib/cylinder.scad>;
use <lib/triangle.scad>;
use <lib/antenna.scad>;
use <stick.scad>;
use <lib/LCD_1602_I2C.scad>;
use <lib/PCB.scad>;
use <lib/Arduino_nano.scad>;

show_switch         =1;
show_lcd            =1;
show_sticks         =1;
show_stm32          =1;
show_antenna        =1;
show_cc2500         =1;
show_battery_charger=1;
show_joysticks      =1;
show_joysticks_pcb  =1;

show_top            =1;
show_bottom         =1;
show_strapholder    =1;


top_bottom_screws=[
    [-50,-50,0],
    [ 50,-50,0],
    [-50, 50,0],
    [ 50, 50,0],
    [ 15.5, 13,0],
    [-15.5, 13,0],
];

name="phschoen";

text_pos=[[-8.5,-15,0], [8.5,-15,0], [-40,50,0], [-20,50,0], [0,39,0], [20,50,0], [40,50,0]   ];
top_text=["on",        "armed",      "mode",     "beeper",   "failsave",  "led",   "prearm"];
bot_text=["off",       "disarmed",    "",         "",        "",          "",      ""    ];

pos_cc2500=[0,14,-7];
pos_stm32=[40.5,-14,-3];
pos_antenna=[0,125/2+8.20,-15];
pos_lcd=[0,-43,-10];
pos_batery_charger=[-48,-15,-5];
pos_strap_holder=[0,0,3];
strap_screw_dist=15;

remote_top_plate_1=[140,110,0];
remote_top_plate_2=[130,110,0];
remote_top_plate_3=[100,131,0];

ps2_pcb_size=[26.15, 34.15, 1.6];

eps=0.1;
$fn=32;

module remote() {

    // animate sticks
    if($t < 0.25)
        sticks($t*4,0);
    else if($t < 0.5)
        sticks(1,($t-0.25)*4);
    else if($t < 0.75)
        sticks(1-($t-0.5)*4,1);
    else
        sticks(0,1-($t-0.75)*4);

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
    if(show_battery_charger) {
        translate(pos_batery_charger)
        rotate([0,0,-90])
        battery_charger();
    }

    if(show_top)
        top_case();
    if(show_bottom)
        bottom_case();

    if(show_strapholder)
        strapholder();
}
module strapholder() {
    eps=0.1;
    translate(pos_strap_holder)
    {
        difference() {
            union() {
                aligned_rounded_cube([2*(strap_screw_dist+5),15,2], r=2, rounding=[1,1,0]);
                aligned_rounded_cube([2*(strap_screw_dist+5),15,6], r=2, rounding=[1,1,1]);
                translate([0,0,5])
                hull() {
                    aligned_rounded_cube([20,15,eps], r=2, rounding=[1,1,0]);
                    aligned_rounded_cube([15,10,5], r=2, rounding=[1,1,1]);
                }
            }
            // strap holder
            translate([0,0,12])
            rotate([0,90,0]) {
                h_off=10;
                rotate_extrude()
                    translate([8,0])
                            circle(d=4);
            }
            // screw mount
            for (i=[-1,1]) {
                translate([i*strap_screw_dist,0,-eps])
                    cylinder(d=3.5, h=100);
                translate([i*strap_screw_dist,0,4-eps])
                    cylinder(d=6, h=100);
            }
        }
    }
}

module sticks(l1,l2)
{
    max_angle=30;
    for(i=[1,-1]) {
        translate([40*i,20,-10]) {
            if (show_sticks) {
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
    difference() {
        thick=3;
        color([0.5,0.8,0,0.8])
        union() {
            hull() {
                aligned_rounded_cube(remote_top_plate_1+[0,0,1],2,[1,1,0],[1,1,0]);
                aligned_rounded_cube(remote_top_plate_2+[0,0,thick],2,[1,1,0],[1,1,0]);
                aligned_rounded_cube(remote_top_plate_3+[0,0,thick],2,[1,1,0],[1,1,0]);

            }
            // lcd screw holder
            translate([pos_lcd[0],pos_lcd[1]-1.2,0])
            translate([-40.6,-20.25,0])
            {
                // screwsholders
                h=9.75;
                translate([0,1,thick-h]){
                    translate([2.5,4,-1]) rotate([0,0,0])
                    difference() {cylinder(d=5.1,h=h); translate([0,0,-eps]) cylinder(d=2.9, h=4);};
                    translate([2.5,35,-1]) rotate([0,0,0])
                    difference() {cylinder(d=5.1,h=h); translate([0,0,-eps]) cylinder(d=2.9, h=4);};
                    translate([78.5,4,-1]) rotate([0,0,0])
                    difference() {cylinder(d=5.1,h=h); translate([0,0,-eps]) cylinder(d=2.9, h=4);};
                    translate([78.5,35,-1]) rotate([0,0,0])
                    difference() {cylinder(d=5.1,h=h); translate([0,0,-eps]) cylinder(d=2.9, h=4);};
                }
            }

            // button holders
            {
                for(i=[0:1:len(text_pos)-1]) {
                    translate([0,0,thick-10])
                    translate(text_pos[i])
                    difference() {
                        aligned_cube([11,10,8]);
                        translate([0,0,-7.25])
                            aligned_cube([8.5,13,10]);
                    }
                }
            }

            // stm32 holder
            {
                translate(pos_stm32){
                    // side holder
                    pcb_size=[52.3, 22.6, 1.6];
                    for(y=[-1,1])
                    for(x=[-1,1]) {
                        translate([x*(pcb_size[0]/2-10),
                                   y*(pcb_size[1]/2),
                                   0.2]){
                            aligned_cube([5,6.25,pcb_size[2]-pos_stm32[2]],[1,1,0]);
                        }
                        translate([x*(pcb_size[0]/2-10),
                                   y*(pcb_size[1]/2+1.625),
                                   pcb_size[2]-2-2]){
                            aligned_cube([5,3.0,2.6],[1,1,0]);
                        }
                    }
                }
            }

            // cc2500 holder
            {
                translate(pos_cc2500){
                    for(y=[-1,1])
                    for(x=[-1,1]) {
                        translate([x*12.75,y*8,1.25]){
                            aligned_cube([5,5,-pos_cc2500[2]],[1,1,0]);
                        }
                        translate([x*13.05,y*8,1.25-2]){
                            aligned_cube([4.4,5,2],[1,1,0]);
                        }
                    }
                }
            }

            // batery charger holder
            {
                pcb_size=[14.75, 26, 1.6];
                translate(pos_batery_charger){
                    for(x=[-1,1]) {
                        translate([0, x*(14.75/2+1),pcb_size[2]+0.2]){
                            aligned_cube([5,6,5],[1,1,0]);
                        }
                        translate([0, x*(14.75/2+2.1),0]){
                            aligned_cube([5,3.75,2],[1,1,0]);
                        }
                    }
                    translate([ -pcb_size[0]/2-6,0,pcb_size[2]+0.2]){
                        aligned_cube([5.5,4,5],[1,1,0]);
                    }
                    translate([ -pcb_size[0]/2-7.25,0,0]){
                        aligned_cube([3,4,2],[1,1,0]);
                    }
                }
            }
            // top_bottom_srews
            {
                h=20;
                d=6;
                for(i=[0:1:len(top_bottom_screws)-1]) {
                    translate(top_bottom_screws[i]-[0,0,h])
                        difference() {
                            translate([0,0,0])
                            {
                                cylinder_flange_sphere($fn=32,r1=d/2, r2=d, h=h);
                                cylinder(d=d,  h=h);
                            }
                            translate([0,0,-eps])
                            cylinder(d=2.6,  h=5);
                        }
                }
            }
            // stap screw cutouts
            translate(pos_strap_holder)
            {
                // screw mount
                for (i=[-1,1]) {
                    translate([i*strap_screw_dist,0,-6])
                    difference(){
                        {
                            aligned_rounded_cube([10,10,4],1,[1,1,1]);
                        }
                        w=6;
                        cylinder($fn=6, r=w / 2 / cos(180 / 6) + 0.05, h=100);
                    }
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
        translate(pos_strap_holder)
        {
            // screw mount
            for (i=[-1,1]) {
                translate([i*strap_screw_dist,0,-7-eps])
                    cylinder(d=3.5, h=100);
            }
        }
        // stick cutouts
        color("orange")
        for(i=[1,-1]) {
            w=32;
            r=14;
            translate([40*i,20,-eps]) {
                //aligned_rounded_cube(size=[w,w,thick+2*eps], r=2, rounding=[1,1,0]);
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
        // buttons cutouts and naming
        translate([0,0,-eps]) {
            translate([0,26,0])
                text_engave(thick, text_size=5, text_=name);

            translate([0,pos_lcd[1]-15,0])
                text_engave(thick, text_size=5, text_="toolbox v1");

            // buttons
            for(i=[0:1:len(text_pos)-1]) {
                translate(text_pos[i])
                button_with_text(thick=thick+2*eps,
                                text_size=4.2,
                                top_text=top_text[i],
                                bottom_text=bot_text[i]);
            }
        }

    }
}

module button_with_text(thick=4,text_size=5, top_text="on",bottom_text="off")
{
    dist_text=10.5;
    cylinder(d=5.8, h=5*thick+2*eps, center=true);


    color("gray")
    translate([0,0,thick-1])
    union() {
        cylinder(d=12, h=1);
        aligned_cube([2.6,8,1],[1,0,0]);
    }

    translate([0,dist_text,0])
        text_engave(thick, text_size=text_size, text_=top_text);
    translate([0,1-dist_text,0])
        text_engave(thick, text_size=text_size, text_=bottom_text);
}

module text_engave(thick, text_size, text_)
{
    color("black")
    translate([0,0,thick-0.5]) {
        linear_extrude(height = 0.75) {
            text(
                    halign="center",
                    valign="center",
                    $fn=$fn,
                    size=text_size,
                    font="Linux Libertine O",
                    text_
            );
        }
    }
}

module bottom_case() {

    h=20;
    wall=3*2;

    difference() {
        color([1,1,1,0.8])
        union() {
            //body itself
            translate([0,0,-h])
            difference() {
                union() {
                    // top hull
                    hull() {
                        aligned_rounded_cube(remote_top_plate_1+[0,0,h],2,[1,1,0],[1,1,0]);
                        aligned_rounded_cube(remote_top_plate_2+[0,0,h],2,[1,1,0],[1,1,0]);
                        // middle plate extended a bit lower
                        translate([0,0,-5])
                        aligned_rounded_cube(remote_top_plate_3+[0,0,h+5],2,[1,1,0],[1,1,0]);
                    }
                    // left and right grip
                    for(i=[1,-1]) {
                        hull() {
                            union() {
                                translate([i*(remote_top_plate_1[1]/2)-i*0,0,-14]) {
                                    aligned_rounded_cube([h,100,20],6 );
                                }
                            }
                            translate([i*remote_top_plate_1[1]/2-i*5,0,0]) {
                                aligned_rounded_cube([h+20,110,0.1],6,[1,1,0]);
                            }
                        }
                    }
                }
                // inner cutout
                color([0.8,0.8,0.8,0.8])
                translate([0,0,wall/2+1])
                hull() {
                    extra_lower_in_mid=5;
                    s=[wall,wall,0];
                    aligned_rounded_cube(remote_top_plate_1 +[0,0,h] - s,2,[1,1,0],[1,1,0]);
                    aligned_rounded_cube(remote_top_plate_2 +[0,0,h]- s,2,[1,1,0],[1,1,0]);
                    translate([0,0,-extra_lower_in_mid])
                    aligned_rounded_cube(remote_top_plate_3 +[0,0,h+extra_lower_in_mid]- s,2,[1,1,0],[1,1,0]);

                }
            }

            // ps2 dust wall
            for(i=[1,-1]) {
                w=42;
                h=24;
                r=10;
                translate([40*i-i*1,20,-0.5-h-eps]) {
                    difference() {
                        aligned_rounded_cube(size=[w-5,w,h], r=r, rounding=[1,1,0]);

                        translate([0,0,+eps])
                        aligned_rounded_cube(size=[w-5-1.5,w-1.5,h+45], r=r, rounding=[1,1,0]);

                        // cable hole
                        translate([1*i,w/2,5])
                        aligned_rounded_cube([10,10,3], r=1, rounding=[1,0,1]);

                    }
                }
            }
            // ps2 holder
            for(i=[1,-1]) {
                h=6;
                translate([0,0,-h])
                translate([40*i,20,-10]) {
                    translate([0,0,-8]) {
                        color("yellow")
                        for(x=[1,-1]) {
                                // screwholes near pin
                                translate([x*(ps2_pcb_size[0]/2-2.7), ps2_pcb_size[1]/2-4.5,-eps])
                                difference() {
                                    cylinder(d=5,h=h);
                                    translate([0,0,h-3])
                                    cylinder(d=2.6,h=3+eps);
                                }
                                // screwholes other side
                                translate([x*(ps2_pcb_size[0]/2-2.7), 2.9-ps2_pcb_size[1]/2,-eps])
                                difference() {
                                    cylinder(d=5,h=h);
                                    translate([0,0,h-3])
                                    cylinder(d=2.6,h=3+eps);
                                }
                        }
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
            pcb_size=[52.3, 22.6, 1.6];
            color("red") {
                w=1.5;
                translate(pos_stm32)
                translate([pcb_size[0]/2+4+w, 0,-1.75-5]) // undo moveing
                {
                    aligned_rounded_cube([8,12,8], 3, [0,1,1]);
                    translate([-w-eps, 0,4]) // undo moveing
                        aligned_cube([8,7,3], [1,1,1]);
                }
            }
        }

        //bot_screw holes
        {
            d=6;
            for(i=[0:1:len(top_bottom_screws)-1]) {
                h_screw=30;
                h_screw2=10;
                translate(top_bottom_screws[i] + [0,0,-h_screw-h-eps])
                    cylinder(d=3,  h=h_screw);
                translate(top_bottom_screws[i] + [0,0, -h_screw2-h-1-2.5])
                    cylinder(d=6,  h=h_screw2);
            }
        }
    }


}
module switch() {
    // screw
    color("silver")
    cylinder(d=5.75,h=8.75);

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

    // screw place
    /* difference() { */
    /*     cylinder(d=10.2, h=1); */
    /*     translate([0,0,-0.1]) */
    /*         cylinder(d=6.1, h=2); */
    /* } */

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

    pcb_size=[52.3, 22.6, 1.6];
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
                aligned_cube(pcb_size);
            }
            // IC
            color("black")
            rotate([0,0,45])
            translate([0,0,pcb_size[2]])
            aligned_cube([7,7,2],[1,1,0]);

            // IC pins
            color("silver")
            rotate([0,0,45])
            translate([0,0,pcb_size[2]])
            aligned_cube([8,8,1],[1,1,0]);

            translate([pcb_size[0]/2,0 ,pcb_size[2]])
            usb_micro();

            // pins pads
            for(p=[0,1])
            for(i=[0:1:len(pin_names[p])-1]) {
                translate([-pcb_size[0]/2+pin_offset[p][0]+i*pitch,
                           -pcb_size[1]/2+pin_offset[p][1],-eps])
                {
                    color("silver") {
                        cylinder(d=1.5,h=pcb_size[2]+2*eps);
                    }
                    color("black") {
                        translate([0,(p-0.5)*2*2,pcb_size[2]+eps])
                        linear_extrude(height = eps)
                        text(
                                halign="center",
                                valign="center",
                                $fn=$fn,
                                size=1,
                                font="Linux Libertine O",
                                pin_names[p][i]
                        );
                    }
                }
            }
        }
        // pin drill hole
        for(p=[0,1])
        for(i=[0:1:len(pin_names[p])-1]) {
            translate([-pcb_size[0]/2+pin_offset[p][0]+i*pitch,
                       -pcb_size[1]/2+pin_offset[p][1],-2*eps])
            color("Silver") {
                cylinder(d=0.7,h=pcb_size[2]+4*eps);
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

    pcb_size=[34.3, 21.4, 0.9];
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
                aligned_cube(pcb_size);
            }
            // hf cage
            color("silver")
            translate([0,0,pcb_size[2]])
            aligned_cube([26.84,19.75,3.2],[1,1,0]);

            // pins pads
            for(i=[0:1:len(pin_names)-1]) {
                translate([-pcb_size[0]/2+pin_offset[0],
                           -pcb_size[1]/2+pin_offset[1]+i*pitch,-eps])
                {
                    color("silver") {
                        cylinder(d=1.5,h=pcb_size[2]+2*eps);
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
                                font="Linux Libertine O",
                                pin_names[i]
                        );
                    }
                }
            }
        }
        // pin drill hole
        for(p=[0,1])
        for(i=[0:1:len(pin_names)-1]) {
            translate([-pcb_size[0]/2+pin_offset[0],
                       -pcb_size[1]/2+pin_offset[1]+i*pitch,-2*eps])
            color("Silver") {
                cylinder(d=0.7,h=pcb_size[2]+4*eps);
            }
        }

    }
}
module battery_charger() {
    eps=0.1;
    pcb_size=[14.75, 26, 1.6];

    // pcb
    color("lightblue")
    aligned_cube(pcb_size);

    // Pins charge
    color("silver") {

        mirror([1,0,0]) {
            translate(pcb_size/2)
            aligned_cube([2.38, 4, pcb_size[2]+eps], [2,2,1]);
        }
        translate(pcb_size/2)
        aligned_cube([2.38, 4, pcb_size[2]+eps], [2,2,1]);
    }

    // Pins bat
    color("silver") {

            mirror([1,0,0]) {
                translate(pcb_size/2 + [-1,-17,0])
                cylinder(d=1.7,h=pcb_size[2]+eps, center=true);
                translate(pcb_size/2 + [-1,-19.5,0])
                cylinder(d=1.7,h=pcb_size[2]+eps, center=true);
            }

            translate(pcb_size/2 + [-1,-17,0])
            cylinder(d=1.7,h=pcb_size[2]+eps, center=true);


            translate(pcb_size/2 + [-1,-19.5,0])
            cylinder(d=1.7,h=pcb_size[2]+eps, center=true);
    }

    translate([0, pcb_size[1]/2-7,pcb_size[2]])
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
                            font="Linux Libertine O",
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
remote();
