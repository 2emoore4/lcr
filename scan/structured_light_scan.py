import Image
import math
from optparse import OptionParser
import os

b = 72 # baseline distance
f = 22 # focal length

min_color = 20
max_color = 235

left_theta = 1.5708
right_theta = 1.01129
frame_count = 96
diff_per_frame = (left_theta - right_theta) / frame_count

def rename_files(dir_name):
	""" This was an elaborate plan to rename image files.

	It was intended to be used to rename n images to
	"scan000, scan001, ... , scan[n-1]" but then I realized I
	didn't need to do this at all, so I didn't even finish
	writing the function. But I'll leave it in here in case I
	need it at some point and want to finish it."""

	# if not dir_name.endswith("/"):
	# 	dir_name = dir_name + "/"

	# if not os.listdir(dir_name):
	# 	print "directory is empty, dummy"
	# else:
	# 	os.mkdir(dir_name + "tmp")
	# 	for filename in os.listdir(dir_name):
	# 		os.rename(filename, "tmp/" + filename)

	print "You've selected the rename flag. It does nothing."

def scan_dir(dir_name):
	print "Processing scans in directory: " + dir_name
	print_divider()
	if not os.listdir(dir_name):
		print "Directory is empty, make sure there are scans here."
	else:
		first_scan = Image.open(dir_name + os.listdir(dir_name)[0])
		print "x size is " + str(first_scan.size[0])
		print "y size is " + str(first_scan.size[1])
		print_divider()

		z_array = [[(0, 0, 0) for x in xrange(first_scan.size[1])] for x in xrange(first_scan.size[0])]

		print "Scanning images and creating z array..."
		print_divider()

		scan_number = 0
		for filename in os.listdir(dir_name):
			scan = Image.open(dir_name + filename)

			for y in xrange(scan.size[1]):
				for x in xrange(scan.size[0]):
					r, g, b = scan.getpixel((x, y))
					if is_white(r, g, b):
						z_array[x][y] = z_triangulation(x, y, scan_number)

			scan_number += 1

		output_heatmap(z_array)

def z_triangulation(x, y, scan_number):
	theta = angle_for_frame(scan_number)
	multiplier = b / (f * (1 / math.tan(theta)) - x)
	return multiplier * x, multiplier * y, multiplier * f

def red_blue_for_z(z, min_z, max_z):
	red = translate(z, min_z, max_z, min_color, max_color)
	blue = translate(z, min_z, max_z, max_color, min_color)

	return red, blue

def translate(value, leftMin, leftMax, rightMin, rightMax):
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    valueScaled = float(value - leftMin) / float(leftSpan)

    return rightMin + (valueScaled * rightSpan)

def output_heatmap(z_array):
	z_list = []
	for u in xrange(len(z_array)):
		for v in xrange(len(z_array[0])):
			x, y, z = z_array[u][v]
			if (z != 0):
				z_list.append(z)

	print "max z is " + str(max(z_list))
	print "min z is " + str(min(z_list))
	print_divider()

	min_z = min(z_list)
	max_z = max(z_list)

	output = Image.new("RGB", (len(z_array), len(z_array[0])), "blue")

	for u in xrange(len(z_array)):
		for v in xrange(len(z_array[0])):
			x, y, z = z_array[u][v]
			if (x != 0 or y != 0 or z != 0):
				red, blue = red_blue_for_z(z, min_z, max_z)
				output.putpixel((u, v), (int(red), 0, int(blue)))

	print "Saving heatmap."
	output.save("heatmap.png")

# this lerp function has an off by one issue somewhere (I think)
def angle_for_frame(frame_number):
	return left_theta - (diff_per_frame * frame_number)

def is_white(r, g, b):
	if r == 255 and g == 255 and b == 255:
		return True
	else:
		return False

def print_divider():
	print "-----------------------------------------------------"

def fix_dir_name(dir_name):
	if dir_name.endswith("/"):
		return dir_name
	else:
		return dir_name + "/"

def clean_dir(dir_name):
	if ".DS_Store" in os.listdir(dir_name):
		os.remove(dir_name + ".DS_Store")

def main():
	parser = OptionParser()
	parser.add_option("-r", action="store_true", dest="rename")
	parser.add_option("-s", action="store_true", dest="scan")
	parser.add_option("-d", "--dir", dest="dir_name")

	(options, args) = parser.parse_args()

	if not options.dir_name:
		print "Need to specify directory with -d option"
	else:
		fixed_dir = fix_dir_name(options.dir_name)
		clean_dir(fixed_dir)

		if options.rename:
			rename_files(fixed_dir)

		if options.scan:
			scan_dir(fixed_dir)

if __name__ == "__main__":
	main()