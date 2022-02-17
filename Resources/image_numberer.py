from PIL import Image, ImageDraw, ImageFont
import sys

img_path = sys.argv[1]
img_pixel_count_x = int(sys.argv[2])
img_pixel_count_y = int(sys.argv[3])
mode = int(sys.argv[4])
print("Image path is : %s" % img_path)
img = Image.open(img_path)
d1 = ImageDraw.Draw(img)
xcounter = 0
ycounter = 0
xsize, ysize = img.size
print("Image size is : %s, %s" % (xsize, ysize))
remainder = xsize % img_pixel_count_x
xsize = xsize - remainder

remainder = ysize % img_pixel_count_y
ysize = ysize - remainder
print("Image xsize : %s, ysize : %s" % (xsize, ysize))
xi = 0
yi = 0
xblocks = int((xsize)/img_pixel_count_x)
yblocks = int(ysize/img_pixel_count_y)
if mode == 0:
    for i in range(yblocks):
        for i in range(xblocks):
            counter_text = "%s%s" % (xcounter, ycounter)
            d1.text((xi, yi), counter_text, fill=(255,0,0))        
            xi = xi + img_pixel_count_x
            if (xi == (xsize)):
                yi = yi + img_pixel_count_y
                xi = 0
            xcounter = xcounter + 1
        ycounter = ycounter + 1
else:
    for i in range(yblocks):
            yi = yi + img_pixel_count_y
            d1.line([(0,yi), (xsize, yi)], fill=128, width=1)
    for i in range(xblocks):
            xi = xi + img_pixel_count_x
            d1.line([(xi,0), (xi, ysize)], fill=128, width=1)


save_img_path = "%s_numbered.png" % img_path[:-4]
img.show()
img.save(save_img_path)