package main

import "fmt"

var b, f int = 72, 22
var left_theta, right_theta, left_alpha, right_alpha float64 = 1.5708, 1.01129, 0.23022, 0.33389
var frame_count int = 96
var image_size_x, image_size_y = 1280, 720
var sub_sampling_rate, expected_x_deviation = 8, 100

func rename_files(dir_name string) {

}

func scan_dir(dir_name string) {

}

func scan_video() {

}

func scan_image(filename string, scan_number int, z_array [][]float64) {

}

func z_triangulation(x int, y int, scan_number int) float64 {
	return 0.0
}

func translate(value float64, left_min float64, left_max float64, right_min float64, right_max float64) float64 {
	return 0.0
}

func output_pcd(z_array [][]float64) {

}

func angle_for_frame(frame_number int) float64 {
	return 0.0
}

func is_white(r int, g int, b int) bool {
	if r == 255 && g == 255 && b == 255 {
		return true
	} else {
		return false
	}
}

func print_divider() {
	fmt.Println("-----------------------------------------------------")
}

func fix_dir_name(dir_name string) {

}

func main() {
	fmt.Println("hi. I don't do anything yet.")
}