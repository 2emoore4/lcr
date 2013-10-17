package main

import "bufio"
import "fmt"
import "image/png"
import "io/ioutil"
import "math"
import "os"
import "strings"
import "time"

var b, f int = 72, 22
var left_theta, right_theta, left_alpha, right_alpha float64 = 1.5708, 1.01129, 0.23022, 0.33389
var frame_count int = 96
var image_size_x, image_size_y = 1280, 720
var sub_sampling_rate, expected_x_deviation = 8, 100

func rename_files(dir_name string) {
	var files, _ = ioutil.ReadDir(dir_name)
	if len(files) == 0 {
		fmt.Println("directory is empty, dummy")
	} else {
		// rename the files. won't need this for now.
	}
}

func scan_dir(dir_name string) {
	fmt.Println("Processing scans in directory: " + dir_name)
	print_divider()

	var files, _ = ioutil.ReadDir(dir_name)
	if len(files) == 0 {
		fmt.Println("Directory is empty, make sure there are scans in here.")
	} else {
		var z_array [1280][720]float64
		for x := 0; x < image_size_x; x++ {
			for y := 0; y < image_size_y; y++ {
				z_array[x][y] = 0
			}
		}

		fmt.Println("Scanning images and creating z array...")
		print_divider()

		start_time := time.Now()

		scan_number := 0
		for i := 0; i < len(files); i++ {
			filename := files[i]
			if strings.Contains(filename.Name(), "png") {
				scan_image(dir_name + filename.Name(), scan_number, z_array)
				scan_number++
			}
		}

		time_elapsed := time.Since(start_time)
		fmt.Println("scan finished in " + time_elapsed.String())
	}
}

func scan_video(filename string) {
	fmt.Println("going to scan video file: " + filename)
}

func scan_image(filename string, scan_number int, z_array [1280][720]float64) {
	fmt.Println("Opening file " + filename)

	file, err := os.Open(filename)
	if err != nil { panic(err) }
	r := bufio.NewReader(file)
	scan_image, err := png.Decode(r)

	start_time := time.Now()

	previous_line_location := -1
	for y := 0; y < image_size_y; y += sub_sampling_rate {
		enter_white, exit_white := 0, 0
		var min_x, max_x int

		if previous_line_location == -1 || previous_line_location < expected_x_deviation || previous_line_location > (image_size_x - expected_x_deviation) {
			min_x, max_x = 0, image_size_x
		} else {
			min_x, max_x = previous_line_location - expected_x_deviation, previous_line_location + expected_x_deviation
		}

		for x := min_x; x < max_x; x++ {
			color := scan_image.At(x, y)
			r, g, b, _ := color.RGBA()
			if is_white(r, g, b) {
				enter_white = x
				break
			}
		}
		for x := enter_white; x < max_x; x++ {
			color := scan_image.At(x, y)
			r, g, b, _ := color.RGBA()
			if !is_white(r, g, b) {
				exit_white = x - 1
				break
			}
		}

		if enter_white != 0 && exit_white != 0 {
			mid_white := int((enter_white + exit_white) / 2)
			previous_line_location = mid_white
			z_array[mid_white][y] = z_triangulation(mid_white, y, scan_number)
		}
	}

	time_elapsed := time.Since(start_time)
	fmt.Println("scanned image in " + time_elapsed.String())
}

func z_triangulation(x int, y int, scan_number int) float64 {
	y = image_size_y - y
	var theta float64 = translate(float64(scan_number), 0.0, float64(frame_count - 1), left_theta, right_theta)
	var alpha float64 = translate(float64(scan_number), 0.0, float64(frame_count - 1), left_alpha, right_alpha)
	var z float64 = float64(b) * (math.Sin(theta) / math.Sin(alpha + theta))
	return z * (-float64(f))
}

func translate(value float64, left_min float64, left_max float64, right_min float64, right_max float64) float64 {
	var left_span float64 = left_max - left_min
	var right_span float64 = right_max - right_min

	var value_scaled = (value - left_min) / (left_span)

	return right_min + (value_scaled * float64(right_span))
}

func output_pcd(z_array [][]float64) {

}

func angle_for_frame(frame_number int) float64 {
	return 0.0
}

func is_white(r uint32, g uint32, b uint32) bool {
	if r == 255 && g == 255 && b == 255 {
		return true
	}

	return false
}

func print_divider() {
	fmt.Println("-----------------------------------------------------")
}

func fix_dir_name(dir_name string) string {
	if strings.HasSuffix(dir_name, "/") {
		return dir_name
	}

	return dir_name + "/"
}

func main() {
	scan_dir("/Volumes/theoc/evan/Google Drive/scan_lines/")
}