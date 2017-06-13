# OpenglGame
This is a simple game implemented with OpenGL. In the game, the scene is made up of several boxes placed in different height. Besides, there are several breads floating on different places of the boxes. The player, in the first-person-perspective, need to move, jump and look around, to look for the breads and eat them!

## 1. Introduction
### 1.1 Result
- Play

     ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/camera.gif)
     
- First-Person-Perspective of the scene

     ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/First-Person-Perspective.png)
     
- Third-Person-Perspective of the scene

     ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/Third-Person-Perspective.png)

### 1.2 Control
- W/A/S/D on keyboard to move.
- Space on keyboard to jump.
- Click the left button of mouse and drag for perspective roaming. 

---

## 2. Coding Environment & 3rd Party Library
- Win10(x86) + VS2015
- C++
- freeglut
- GLEW
- Assimp
- DevIL
- SOIL

---

## 3. Functionalities
- Basic functionalities
    - Camera Roaming
    - Texture Mapping
    - Simple Lighting
    - Sky Box
    - Deep Test
    - Model Managment
    
- Additional features
    - Scene switching
    - Gravity simulation & Collision detection
    - Stencil Test
    - Display Text (En/Cn)
    - Complex Texture (Bumpmap)
    - Particle System & Explosion Effect
    
---

## 4. Additional Features' Detail
1. __Scene switching__:  When starting the program, we will enter the home page, containing the game rules and a _Start_ button. Clicking the button, and we will enter the main game scene. (You can see the effect from the GIF above)

1. __Gravity simulation & Collision detection__:  see the code explanation in Chinese version: http://blog.csdn.net/qq_33000225/article/details/72861956

     ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/gravity.gif)
     
1. __Stencil Test__:  We use Stencil Test to draw the edge of the bread.

     ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/stencilTesting.png)
     
1. __Display Text__:  We have English and Chinese text on the home page. At the game scene, a string text is placed on the upper left corner of the window to show how many breads the player has eaten. Keeping the text at the fixed position is relavant with the camera roaming.

1. __Complex Texture__:  We use bumpmap here to make the boxes more stereoscopic.

    ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/bumpmap.png)
    
1. __Particle System & Explosion Effect__:  When the player eats the bread, an explosion effect with several particles will be generate  ahead.

    ![Image text](https://github.com/MarkMoHR/OpenglGame/raw/master/ResultScreenshots/explosion.png)
