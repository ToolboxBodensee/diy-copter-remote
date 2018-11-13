module triangle(o_len=5, a_len=5, depth=1)
{
    linear_extrude(height=depth) {
        polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
    }
}
module aligned_triangle(o_len=5, a_len=5, depth=1)
{
    linear_extrude(height=depth) {
        translate([-o_len/2,-a_len/2,0])
            polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
    }
}
