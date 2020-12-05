#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

    
    //Salvar pontuações
            //Estrutura 
            struct score {
                string nome;
                int pontos;
            };
            score pontuacoes;

            bool cmp(const score &s1, const score &s2) {
                return s1.pontos > s2.pontos || (s1.pontos == s2.pontos && s1.nome < s2.nome);
            }


int main() {
    


            //Vetor para salvar os dados da estrutura
            vector <score> highscores;


    //Ler partidas anteriores
    ifstream entrada;
    entrada.open("highscores.txt");

    if (!entrada){
        entrada.close();
        }

    while (entrada >> pontuacoes.nome) {
        entrada >> pontuacoes.pontos;
        highscores.push_back(pontuacoes);
    }
    entrada.close();            

    //Tamanhos
            //Tamanho do Cano
            const int pipeWidth = 104, pipeHeight = 640;

            //Tamanho do Passaro
            const int birdWidth = 176, birdHeight = 162;
            const int birdRadius = 20;

            //Tamanho da Janela
            const int windowWidth = 900, windowHeight = 900;
            sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Flappy Bird");



    //Limite de Frames
    window.setFramerateLimit(60);
    sf::Clock clock;



    //Sprites
            //Sprite do Fundo
            sf::Texture bgTexture;
            bgTexture.loadFromFile("assets/background.png");
            bgTexture.setRepeated(true);
            sf::IntRect bgRect(0,1080-windowHeight, windowWidth, windowHeight);
            sf::Sprite bgSprite(bgTexture);

            //Sprite do Passáro
            sf::Texture birdTexture;
            birdTexture.loadFromFile("assets/birdTexture.png");
            sf::IntRect birdRect(0,0, birdWidth, birdHeight);
            sf::Sprite birdSprite(birdTexture, birdRect);
            birdSprite.setPosition(windowWidth/2, windowHeight/2);
            birdSprite.setOrigin(birdWidth/2, birdHeight/2);

            //Hitbox do Passáro
            sf::CircleShape shape((birdRadius*2));
            shape.setOrigin (birdRadius, birdRadius);
            shape.setFillColor(sf::Color::Green);

            //Sprite do Cano de baixo
            sf::Texture pipeDownTexture;
            pipeDownTexture.loadFromFile("assets/pipe_down.png");
            sf::IntRect pipeDownRect(0,0,pipeWidth,pipeHeight);
            sf::Sprite pipeDownSprite(pipeDownTexture, pipeDownRect);
            pipeDownSprite.setTextureRect(pipeDownRect);
            pipeDownSprite.setPosition(windowWidth, windowHeight/2);

            //Sprite do Cano de Cima
            sf::Texture pipeUpTexture;
            pipeUpTexture.loadFromFile("assets/pipe_up.png");
            sf::IntRect pipeUpRect(0,0,pipeWidth,pipeHeight);
            sf::Sprite pipeUpSprite(pipeUpTexture, pipeUpRect);
            pipeUpSprite.setTextureRect(pipeUpRect);
            pipeUpSprite.setPosition(windowWidth, windowHeight/2);

            //Sprite do Chão
            sf::Texture groundTexture;
            groundTexture.loadFromFile("assets/base.png");
            groundTexture.setRepeated(true);
            sf::IntRect groundRect(0,0, windowWidth, 100);
            sf::Sprite groundSprite(groundTexture, groundRect);
            groundSprite.setPosition(0, windowHeight-100);

            //Teto da tela
            sf::RectangleShape roof;
            roof.setSize(sf::Vector2f(windowWidth, 1));
            roof.setPosition(0,0);



    //Textos
    sf::Font font;
    font.loadFromFile("assets/fontt.ttf");
    
            //Pontos
            sf::Text pointstext;
            pointstext.setFont(font);
            pointstext.setCharacterSize(45);
            pointstext.setFillColor(sf::Color::White);
            pointstext.setPosition(windowWidth/2, 50);

            //Nickname input
            sf::String playerInput;
            sf::Text playerText;
            playerText.setFont(font);
            playerText.setCharacterSize(25);
            playerText.setFillColor(sf::Color(150, 150, 150));
            playerText.setPosition(windowWidth/2-170, 255);

            //Nickname string
            sf::Text nicktext;
            nicktext.setFont(font);
            nicktext.setCharacterSize(30);
            nicktext.setFillColor(sf::Color::Black);
            nicktext.setPosition(windowWidth/2-400, 250);
            nicktext.setString("NICKNAME: ");



    //Menus
            //Menu de game over
            sf::Texture gameoverTexture;
            gameoverTexture.loadFromFile("assets/gameover2.png");
            sf::Sprite gameoverSprite(gameoverTexture);
            gameoverSprite.setPosition(windowWidth/4, 130);

            //Menu inicial
            sf::Texture menuTexture;
            menuTexture.loadFromFile("assets/title.png");
            sf::Sprite mainmenuSprite(menuTexture);
            mainmenuSprite.setPosition(200, 50);



    //Variaveis úteis para o game
        bool gameRunning = false, initialMenu = true, gameOver = false, jump = false, spriteLimiter = false, pointGotten = false;
        int y = windowHeight/2, jumpCount = 0, rotation = 0, spriteCount = 0, pipex = windowWidth, pipesheight = windowHeight/2, points = 0;
        float pipeTime = 10.2, pipeSpeed = 5;        
        string nickname;





                                        //Game Loop
    while (window.isOpen())
    {
        sf::Event event;
        
        //Tratamento de Eventos
        while (window.pollEvent(event))
        {
                //Fechar Janela com o botão no canto superior direito
                if (event.type == sf::Event::Closed)
                    window.close();

                //Entrada de texto para o nickname
                else if (event.type == sf::Event::TextEntered) {
                    if (playerInput.getSize() < 10) {
                        playerInput.replace(sf::String(" "), sf::String(""));
                        playerInput.replace(sf::String(""), sf::String(""));
                        playerInput +=event.text.unicode;
                        playerText.setString(playerInput);
                        nickname = playerText.getString();
                    }
                }

                //Eventos de KeyPressed
                else if (event.type == sf::Event::KeyPressed){

                        //Fechar jogo usando "Esc"
                        if(event.key.code == sf::Keyboard::Escape){
                            window.close();
                        }

                        //Eventos usando "Space"
                        else if (event.key.code == sf::Keyboard::Space){

                            //Iniciar jogo
                            if (!gameRunning && !gameOver && playerInput.getSize() > 2) {
                                pontuacoes.nome = nickname;
                                playerInput.clear();
                                gameRunning = true;
                                gameOver = false;
                                initialMenu = false;
                                pipeSpeed = 5;
                                pointstext.setString("0");
                                y = windowHeight/2;
                                rotation = 0;
                                pipex = windowWidth;
                                pipeDownSprite.setPosition(pipex, pipesheight);
                                pipeUpSprite.setPosition(pipex, pipesheight-pipeHeight-350);
                                birdRect.left = birdWidth;
                                birdRect.top = 0;
                                birdSprite.setTextureRect(birdRect);
                            }

                            //Iniciar menu de gameover
                            else if (gameOver) {
                                pontuacoes.pontos = points;
                                highscores.push_back(pontuacoes);
                                playerInput.clear();
                                initialMenu = true;
                                gameOver = false;
                                y = windowHeight/2;
                                rotation = 0;
                                spriteCount = 0;
                                jumpCount = 0;
                                points = 0;
                                pipex = windowWidth;
                                pipeDownSprite.setPosition(pipex, pipesheight);
                                pipeUpSprite.setPosition(pipex, pipesheight-pipeHeight-350);
                                birdRect = {0,0, birdWidth, birdHeight};
                                birdSprite.setPosition(windowWidth/2, windowHeight/2);
                                birdSprite.setTextureRect(birdRect);
                                pointstext.setString("");
                            }

                            //Pular com o pássaro
                            else if(jumpCount < 10){
                                jump = true;
                                jumpCount = 25;
                                spriteCount = 3;
                                birdRect.left = birdWidth*spriteCount;
                                birdSprite.setTextureRect(birdRect);
                            }
                        }
                    }
                }

                                        //Jogo Rodando
        //Mover background
        if(gameRunning || initialMenu) {
            bgRect.left = bgRect.left +1;
            bgSprite.setTextureRect(bgRect);
        }

        //Propriedades do Jogo rodando
        if(gameRunning){
                    //Gravidade p/ o pássaro
                    y = y + 10;
                    if(rotation < 90) rotation = rotation + 1.5;
                    birdSprite.setPosition(windowWidth/2, y);
                    shape.setPosition(windowWidth/2, y);
                    birdSprite.setRotation(rotation);
                    
                    //Canos Aleatórios
                    pipeDownSprite.setPosition(pipex, pipesheight);
                    pipeUpSprite.setPosition(pipex, pipesheight-pipeHeight-350);
                    pipex -= pipeSpeed;
                    groundRect.left += pipeSpeed;
                    groundSprite.setTextureRect(groundRect);
                    if (pipex <= 0 - pipeWidth) {
                        pipesheight = rand() % 400 + 375;
                        pipex = windowWidth;
                        pointGotten = false;
                    }

                    //Batidas
                            //Batida no teto
                            if(shape.getGlobalBounds().intersects(roof.getGlobalBounds())){
                                y = 0 + birdRadius;
                            }
                                
                            //Batida mortal
                            if(shape.getGlobalBounds().intersects(pipeUpSprite.getGlobalBounds()) 
                                || shape.getGlobalBounds().intersects(pipeDownSprite.getGlobalBounds())
                                || shape.getGlobalBounds().intersects(groundSprite.getGlobalBounds()))
                            {
                                gameRunning = false;
                                gameOver = true;
                                birdRect.left = birdWidth;
                                birdRect.top = birdHeight;
                                birdSprite.setTextureRect(birdRect);
                            }

                    //Marcar Pontos
                    if(shape.getPosition().x >= pipeDownSprite.getPosition().x 
                        && (pipeDownSprite.getPosition().x + pipeWidth) >= shape.getPosition().x 
                        && pointGotten == false)
                    {
                        points++;
                        pointstext.setString(std::to_string(points));
                        pipeSpeed += 1;
                        pointGotten = true;
                    }
                }

                //Estado do Pulo
                if (jump && jumpCount > 0) {
                    y = y - jumpCount;
                    if(rotation >= -45) rotation = rotation - (jumpCount/5);
                    jumpCount--;
                    if(spriteCount > 0 && spriteLimiter) spriteCount --, spriteLimiter = false;
                    else spriteLimiter = true;
                    birdRect.left = birdWidth*spriteCount;
                    birdSprite.setTextureRect(birdRect);
                }






        //Desenhar frames        
        window.clear();
        window.draw(shape);
        window.draw(roof);
        window.draw(bgSprite);
        window.draw(pipeDownSprite);
        window.draw(pipeUpSprite);
        window.draw(groundSprite);
        window.draw(birdSprite);
        window.draw(pointstext);
        if (gameOver) {
            window.draw(gameoverSprite);
        }
        if (initialMenu) {
            window.draw(mainmenuSprite);
            window.draw(playerText);
            window.draw(nicktext);
        }
        window.display();
    }

                                        //Salvando pontuação num arquivo txt
    sort(highscores.begin(), highscores.end(), cmp);

        ofstream saida;
        saida.open("highscores.txt");

        if (!saida){
            cout << "Falhou";
            exit(1);
        }

        for (auto h : highscores){
            saida << h.nome << " " << h.pontos << endl;
        }
        saida.close();

        

        

    return 0;
}