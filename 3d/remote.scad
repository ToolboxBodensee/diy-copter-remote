use <lib/cube.scad>;
use <lib/cylinder.scad>;
use <lib/triangle.scad>;
use <lib/antenna.scad>;
use <stick.scad>;
use <lib/LCD_1602_I2C.scad>;
use <lib/PCB.scad>;
use <lib/Arduino_nano.scad>;

show_switch=1;
show_lcd=1;
show_sticks=1;
show_stm32=1;
show_antenna=1;
show_cc2500=1;
show_battery_charger=1;

name="phschoen";

text_pos=[[-10,-18,0], [10,-18,0], [-55,49,0], [-25,49,0], [0,40,0], [25,49,0], [55,49,0]   ];
top_text=["on",        "armed",    "some",     "thing",    "needs",  "to be",   "configur"];
bot_text=["off",       "disarmed", "",         "",         "",       "",        ""          ];

pos_cc2500=[0,10,-3];
pos_stm32=[48,-15,-1];
pos_antenna=[0,70,-15];
pos_lcd=[0,-44,-10];
pos_batery_charger=[-48,-15,-5];

eps=0.1;
$fn=32;

module lcd_1602() {
    pcb=[80,36,1.6];
    metal_case=[71.2, 24.2, 6.5];
    case_lcd_cut=[64.5, 14.5, 6.5+2*eps];

    // metal case
    difference() {
        translate([0,0,pcb[2]])
            aligned_cube(metal_case,[1,1,0]);

        translate([0,0,pcb[2]])
            aligned_cube(case-2,[1,1,0]);

        // glas cut
        translate([0,0,pcb[2]-eps])
            aligned_cube(case_lcd_cut,[1,1,0]);
    }

    // glas cut
    color("blue")
    translate([0,0,0])
    aligned_cube(case_lcd_cut,[1,1,0]);

    // pcb
    color("green")
    aligned_cube(pcb,[1,1,0]);
}



module ps2_pad() {
    // pcb
    difference()
    {
        pcb=[26, 34, 1.6];
        color([0.2, 0.2, 0.2]) {
            aligned_cube(pcb,[0,0,0]);
        }

        // mounting holes
        r=3.4/2;
        color([0.8, 0.8, 0.8])
        for (i = [
                    [         1.4+r,          3.3+r, -0.1], // bot left
                    [pcb[0]-(1.4+r),          3.3+r, -0.1], // bot right
                    [pcb[0]-(1.4+r), pcb[1]-(1.6+r), -0.1], // top right
                    [       (1.4+r), pcb[1]-(1.6+r), -0.1], // top left
        ]) {
            translate(i)
                cylinder($fn=16, r=r, h=0.2 + pcb[2]);
        }
    }

    // pins

    {
    }

}
//ps2_pad();
//pin_angled();

module pin_angled(pins=4, angled=true) {
    w=0.65;
    pin_angled=[w, 7-w, w];
    pin_vertical=[w, w, 7.2-w];

    plastic=[2.16,12.6];

    color([0.8,0.8,0.8]) {
        // vertical pin part
        translate([0,0,0])
            aligned_cube(pin_vertical, [1,0,0]);

        // angled pin part
        translate([0,w,7.2])
            aligned_cube(pin_angled, [1,0,2]);

        difference() {
            d=1.5;

            translate([0,d/2,7.2-d/2])
            rotate([0,90,0])
            translate([0,0,-w/2])
            cylinder($fn=32,d=d, h=w);

            translate([0,w+d/2,7.2-d/2-w])
            rotate([0,90,0])
            translate([0,0,-w/2-eps])
            cylinder($fn=32,d=d, h=w+2*eps);
        }
    }
}


module remote() {
    if (show_sticks) {
        if($t < 0.25)
            sticks($t*4,0);
        else if($t < 0.5)
            sticks(1,($t-0.25)*4);
        else if($t < 0.75)
            sticks(1-($t-0.5)*4,1);
        else
            sticks(0,1-($t-0.75)*4);
    }
    if(show_stm32) {
        translate(pos_stm32) {
            rotate([0,180,180])
            stm32_bluepill();
        }
    }

    if(show_antenna) {
        translate(pos_antenna) {
            rotate([90,0,0])
            color("darkgray")
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
        LCD_1602_I2C(PinsDown,
                      "RSSI 99     A99 ",
                      " ARMED      A99 ");
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

    top_case();
    //bottom_case();

}

module sticks(l1,l2)
{
    max_angle=30;
    color("orange")
    for(i=[1,-1]) {
        translate([40*i,20,-10]) {
            rotate([max_angle*(l1*2-1),max_angle*(l2*2-1),0])
            translate([0,0,8]) {
                stick(h=20);
            }
        }
    }
}

module top_case() {
    difference() {
        thick=2.5;
        color([0.5,0.8,0,0.8])
        union() {
            hull() {
                remote_top_plate_1=[150,110,1];
                remote_top_plate_2=[140,120,thick];
                remote_top_plate_3=[100,135,thick];

                aligned_rounded_cube(remote_top_plate_1,2,[1,1,0],[1,1,0]);
                aligned_rounded_cube(remote_top_plate_2,2,[1,1,0],[1,1,0]);
                aligned_rounded_cube(remote_top_plate_3,2,[1,1,0],[1,1,0]);

            }
            {
                for(i=[0:1:len(text_pos)-1]) {
                    translate(text_pos[i])
                    button_with_text(thick=thick+2*eps,
                                    text_size=4,
                                    top_text=top_text[i],
                                    bottom_text=bot_text[i]);
                }
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
                            aligned_cube([5,6.25,pcb_size[2]-0.5],[1,1,0]);
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
                            aligned_cube([5,5,4],[1,1,0]);
                        }
                        translate([x*13.1,y*8,1.25-2]){
                            aligned_cube([4.3,5,2],[1,1,0]);
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

        }
        //lcdcutout
        translate([0,-45,-eps]) {
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
        // stick cutouts
        color("orange")
        for(i=[1,-1]) {
            w=36;
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
            translate([0,61,0])
                text_engave(thick, text_size=5, text_=name);

            translate([0,-61,0])
                text_engave(thick, text_size=5, text_="toolbox v1");

            // buttons
            for(i=[0:1:len(text_pos)-1]) {
                translate(text_pos[i])
                button_with_text(thick=thick+2*eps,
                                text_size=4,
                                top_text=top_text[i],
                                bottom_text=bot_text[i]);
            }
        }

    }
}

module button_with_text(thick=4,text_size=5, top_text="on",bottom_text="off")
{
    dist_text=10.5;
    cylinder(d=5.8, h=thick+2*eps);


    //
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
            translate([0,dist_text,0]) {
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
}

module bottom_case() {

    h=25;
    wall=3*2;
    remote_top_plate_1=[150,110,h];
    remote_top_plate_2=[140,120,h];
    remote_top_plate_3=[100,135,h];

    translate([0,0,-h])
    color([1,1,1,0.1])
    difference() {
        //body itself
        hull() {
            aligned_rounded_cube(remote_top_plate_1,2,[1,1,0],[1,1,0]);
            aligned_rounded_cube(remote_top_plate_2,2,[1,1,0],[1,1,0]);
            aligned_rounded_cube(remote_top_plate_3,2,[1,1,0],[1,1,0]);
        }

        translate([0,0,3])
        hull() {
            aligned_rounded_cube(remote_top_plate_1-[wall,wall,0],2,[1,1,0],[1,1,0]);
            aligned_rounded_cube(remote_top_plate_2-[wall,wall,0],2,[1,1,0],[1,1,0]);
            aligned_rounded_cube(remote_top_plate_3-[wall,wall,0],2,[1,1,0],[1,1,0]);
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
remote();
