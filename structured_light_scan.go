package main

import "fmt"

func rename_files(dir_name string) {

}

func scan_dir(dir_name string) {

}

func scan_video() {

}

func scan_image(filename string, scan_number int, z_array [][]float64) {

}

func z_triangulation(x int, y int, scan_number int) float64 {

}

func translate(value float64, left_min float64, left_max float64, right_min float64, right_max float64) float64 {

}

func output_pcd(z_array [][]float64) {

}

func angle_for_frame(frame_number int) float64 {

}

func is_white(r int, g int, b int) bool {
	if r == 255 && g == 255 && b == 255 {
		return true
	} else {
		return false
	}
}

func print_divider() {

}

func fix_dir_name(dir_name string) {

}

func clean_dir(dir_name string) {

}

func main() {
	fmt.Println("hi. I don't do anything yet.")
}