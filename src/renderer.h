#pragma once

#include "utility.h"
#include "scene.h"
#include "camera.h"
#include "bee_eye.h"
#include "sampler.h"

namespace BeeView {

	class Renderer
	{
	private:
		std::shared_ptr<Camera> m_camera;

	public:
		std::shared_ptr<Scene> m_scene;

		Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) : m_scene(scene), m_camera(camera) 
		{ 
			if(m_scene != nullptr)
				m_scene->initEmbree();
		}

		/* renders the image of scene according to set camera */
		std::unique_ptr<Image> renderToImage();

		/* switch camera */
		void setCamera(std::shared_ptr<Camera> camera)
		{
			m_camera = camera;
		}

	private:

		/* render normal image according to parameters in pinholecamera*/
		std::unique_ptr<Image> renderToImagePinhole();

		/* renders the bee eyes according to parameters in beeEyeCamera */
		std::unique_ptr<Image> renderToImageBeeEye();

		std::unique_ptr<Image> Renderer::renderToImagePanoramic();
	
		/* PINHOLE: renders single pixel */
		Color renderPixel(float x, float y);

		/* Shoot a ray from camera position in direction dir, return texture color at hitpoint */
		Color shootRay(const Vec3f &dir);

		/* returns a pseudo random number calsculated from ID (very not random) */
		Color randomColor(const int ID);

		/* returns color where azimuth is the red part and elevation the blue part */
		Color azimuthElevationColor(const int a, const int e);

		/* renders the given beeeye onto the given image */
		void renderBeeEye(std::unique_ptr<Image> &img, Side side);

		struct ConvertCoordsParams
		{
			int x_min; // min x of beyeye
			int x_max;
			int y_min;
			int y_max;
		};

		/* converts ommatidium.m_x and ommatidium.m_y to image coordinates, saves them in out_x, out_y */
		void convert2ImageCoords(const Ommatidium &ommatidium, const ConvertCoordsParams &params, int &out_x, int &out_y);

		/* draw a square a x,y (top left corner of sqaure) position, size a square side length */
		void drawSquare(std::unique_ptr<Image> &img, const int x, const int y, const int size, Color color)
		{
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					img->set(x + i, y + j, color);
		}

		/* draw a cross at x,y position */
		void drawCross(std::unique_ptr<Image> &img, const int x, const int y, const int length = 6, Color color = Color(0,1,0))
		{
			for (int i = 0; i < length; i++)
			{
				img->set(x - i, y, color);
				img->set(x + i, y, color);
				img->set(x, y - i, color);
				img->set(x, y + i, color);
			}
			return;
		}

	};

}