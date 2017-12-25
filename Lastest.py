import numpy as np
import cv2


hist_size = 256
hist_w = 512
hist_h = 400
hist_thickness = 2
bin_w = int(hist_w / hist_size)

color = ('b', 'g', 'r')
clr = {"b": (255, 0, 0),
       "g": (0, 255, 0),
       "r": (0, 0, 255)}

srcs = ["./src_images/beach_1.jpg",
        "./src_images/beach_2.jpg",
        "./src_images/beach_3.jpg",
        "./src_images/mountain_1.jpg",
        "./src_images/mountain_2.jpg",
        "./src_images/mountain_3.jpg",
        "./src_images/restaurant_1.jpg",
        "./src_images/restaurant_2.jpg",
        "./src_images/restaurant_3.jpg",
        "./src_images/road_1.jpg",
        "./src_images/road_2.jpg",
        "./src_images/road_3.jpg"]


def calculate_histogram(img):
    hist = []
    for i, col in enumerate(color):
        hist.append(cv2.calcHist([img], [i], None, [hist_size], [0, hist_size]))
        cv2.normalize(hist[i], hist[i], 0, hist_w, cv2.NORM_MINMAX, -1)
    return hist


def draw_histogram(hist):
    hist_img = np.zeros((hist_h, hist_w, 3), np.uint8)
    for c, col in enumerate(color):
        hist_img = cv2.line(hist_img,
                            (bin_w * (0), hist_h - hist[c][0]),
                            (bin_w * (0), hist_h - hist[c][0]),
                            clr[col], hist_thickness)
        for i, v in enumerate(hist[c][1:]):
            hist_img = cv2.line(hist_img,
                                (bin_w * (i-1), hist_h - hist[c][i-1]),
                                (bin_w * (i), hist_h - hist[c][i]),
                                clr[col], hist_thickness)
    return hist_img


def main():
    for src in srcs:
        img = cv2.imread(src)
        hist_img = draw_histogram(calculate_histogram(img))
        cv2.imwrite(src + ".hist.png", hist_img)


if __name__ == '__main__':
    main()
