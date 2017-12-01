require 'gosu'
require_relative 'score_editor.rb'
require_relative '../game-object/game_object.rb'

include ScoreEditor

# Define IDs para diferentes estagios do programa
module Stages
  # Estagio de leitura de nome do usuario
  GETNAME = 0

  # Estagio do menu principal
  MENU = 1

  # Estagio de exibicao do ranking
  SCOREBOARD = 2

  # Estagio de execucao do jogo
  GAME = 3
end

# Classe principal de execucao do programa
class Window < Gosu::Window
  attr_accessor :stage

  # Construtor da janela de jogo
  # @param width Largura da janela
  # @param height Altura da janela
  def initialize(width, height)
    super
    self.caption = 'Desert Falcon'
    # Estagio 0 significa menu
    @stage = Stages::MENU
    @menu_image = Gosu::Image.new('assets/image/menu.jpg')
    @background_image = Gosu::Image.new('assets/image/sand.jpg')
    @font = Gosu::Font.new(self, 'assets/text/pixelade-webfont.ttf', 50)

    # Inicializa variaveis do jogo
    init_game
  end

  # Inicializa a mudanca para o estagio de mostrar o ranking
  def show_scoreboard
    # Destaca o placar em tela, mudando o estagio e background
    @stage = Stages::SCOREBOARD
    @menu_image = Gosu::Image.new('assets/image/space.png')
    ScoreEditor.read
  end

  # Interpreta input do teclado de acordo com a entrada de um nome de usuario.
  # @param char_read ID da tecla apertada pelo usuario
  def get_player_name(char_read)
    # Funcao para ler o nome do jogador a partir de um id apertado
    if char_read == 40
      # Caso o usuario tenha apertado enter
      # Adicione-o e mostre o placar
      show_scoreboard
      ScoreEditor.add(@player_name, @inst_falcon.score)
      @scores = ScoreEditor.sort
      ScoreEditor.write(@scores)
      # Como o falcao morreu, reinicialize as variaveis com init_game.
      init_game
    elsif char_read == Gosu::KbEscape
      close
    elsif char_read == 42
      # Caso tenha sido apertado o backspace, retirar o ultimo elemento
      @player_name = @player_name.chop
    elsif @player_name.size < 3
      # Se o tamanho do nome nao tiver chegado a 3, insira o caractere nele.
      @player_name += (char_read + 93).chr
    end
  end

  # Escuta por teclas apertadas durante a execucao da janela.
  # @param id ID da tecla apertada pelo usuario
  def button_down(id)
    if @stage == Stages::GETNAME
      get_player_name(id)
      return
    end

    case id
    when Gosu::Kb1
      # Caso pressionado '1' e se estah no menu, va para o jogo
      @stage = Stages::GAME if @stage == Stages::MENU
    when Gosu::Kb2
      # Caso '2' va para o placar
      if @stage == Stages::MENU
        show_scoreboard
        # Recebe placar ordenado
        @scores = ScoreEditor.sort
      end
    when Gosu::Kb3, Gosu::KbEscape
      # Caso pressionado '3' durante o menu, termine a execucao
      close if @stage == Stages::MENU

      # Caso pressionado '3' durante o jogo, pause-o
      @menu_image = Gosu::Image.new('assets/image/menu.jpg')
      @stage = Stages::MENU
    else
      @inst_falcon.button_down(id) if @stage == Stages::GAME
    end
  end

  # Adiciona um hiero no array da classe
  def add_hiero
    @hieros << Hiero.new(rand(300..600), - Hiero::HEIGHT, 0)
  end

  # Adiciona um inimigo no array da classe
  def add_enemy
    @enemies << Enemy.new(rand(300..600), - Hiero::HEIGHT, rand(1..3))
  end

  # Adiciona um obstaculo no array da classe
  def add_obstacle
    @obstacles << Obstacle.new(rand(200..800), - Obstacle::HEIGHT, 0)
  end

  # Trata de spawnar novos GameObjects
  def spawn_time
    # current_seconds indica quanto tempo se passou desde o inicio do jogo
    current_seconds = (Time.now - @initial_time).floor

    # Last_call representa a ultima chamada feita
    # Eh usada para reduzir o processamento nessa funcao
    return if @last_call == current_seconds

    if (current_seconds % 4).zero?
      # Spawna um hiero a cada 4 segundos
      @last_call = current_seconds
      add_hiero
      return
    elsif (current_seconds % 3).zero?
      # Spawna um obstaculo a cada 3 segundos
      @last_call = current_seconds
      add_obstacle
      return
    elsif (current_seconds % 2).zero?
      # Spawna um inimigo a cada 2 segundos
      @last_call = current_seconds
      add_enemy
      return
    end
  end

  # Atualiza uma array de GameObjects do jogo.
  # @param array Array a ser atualizada
  # @param id Identificador do tipo dos objetos da array
  def update_array(array, id)
    return if array.empty?

    array.each_with_index do |member, idx|
      # Para cada instancia, verifique caso seja valida
      unless member.valid
        array.delete_at(idx)
        next
      end

      # Se esta instancia for valida, mova-a
      member.update
      # Caso exista colisao com o Falcon trate-a
      if @inst_falcon.notify_collision(member, id)
        # Delete-a
        if id == Id::HIERO
          # Se colidir com hiero, aumenta score
          array.delete_at(idx)
          @inst_falcon.score += 1
        elsif id != Id::FALCON
          # Se colidir com obstaculo, game-over
          sleep 0.5
          @stage = Stages::GETNAME
        end
      end
    end
  end

  # Inicializa todas as variaveis relacionadas ao jogo
  def init_game
    # "inst_falcon" eh a instancia de Falcon que sera manipulada nesse modulo
    @inst_falcon = Falcon.new(30, 300, 1)

    # hieros eh um array que conterah instancias da classe Hiero
    @hieros = []

    # obstacles eh um array que conterah instancias da classe Obstacle
    @obstacles = []

    # enemies eh um array que conterah diversas instancias da classe Enemy
    @enemies = []
    @initial_time = Time.now
    @last_call = -1
    @player_name = ''
  end

  # Atualiza a logica do jogo
  def update
    case @stage
    when Stages::GETNAME, Stages::MENU, Stages::SCOREBOARD
      # Nao faz update caso nao seja necessario
      return
    when Stages::GAME
      # Spawn_time spawna o que for necessario de acordo com o tempo
      spawn_time
      # Atualiza o Falcon
      @inst_falcon.update
      # Atualiza todos os hieros
      update_array(@hieros, Id::HIERO)
      # Atualiza todos os objetos(piramides...)
      update_array(@obstacles, Id::OBSTACLE)
      # Atualiza todos os inimigos
      update_array(@enemies, Id::ENEMY)
    end
  end

  # Renderiza as entidades na tela
  def draw
    case @stage
    when Stages::GETNAME
      # Renderiza a funcao que pede pelo nome do usuario ao fim do jogo
      @menu_image = Gosu::Image.new('assets/image/space.png')
      @menu_image.draw(0, 0, -10)
      @font.draw('Game over!', 150, 0, 1, 1.5, 1.5, Gosu::Color::RED)
      # Printa o placar do mesmo
      score_text = "Score #{@inst_falcon.score}"
      @font.draw(score_text, 25, 100, 1, 1.0, 1.0, Gosu::Color::WHITE)
      @font.draw('Digite seu nome: ', 25, 140, 1, 1.0, 1.0, Gosu::Color::WHITE)

      unless @player_name.empty?
        # Imprima o que estiver sendo digitado
        # Desde que nao seja nada
        @font.draw(@player_name, 300, 140, 1, 1.0, 1.0, Gosu::Color::YELLOW)
      end
    when Stages::MENU
      @menu_image.draw(0, 0, -10)
    when Stages::SCOREBOARD
      # Caso placar tenha sido selecionado
      # Printe cada posicao do mesmo
      big_font = Gosu::Font.new(self, 'assets/text/pixelade-webfont.ttf', 80)
      big_font.draw('Placar', 180, 10, 1, 1.0, 1.0, Gosu::Color::WHITE)
      y = 95
      @scores.each_with_index do |member, idx|
        # Printa os membros do placar ate o indice 10
        break if idx >= 10
        score_line = "#{idx + 1}: #{member[0]} #{member[1]}"
        @font.draw(score_line, 200, y, 1, 1.0, 1.0, Gosu::Color::WHITE)
        # Y representa a coordenada y que estah sendo printada
        y += 30
      end

      exit_message = 'Digite \'3\' ou \'Esc\' para retornar ao menu.'
      @font.draw(exit_message, 0, y + 20, 1, 0.9, 0.9, Gosu::Color::YELLOW)
    when Stages::GAME
      # Desenha as entidades:
      @background_image.draw(0, 0, -10)
      @inst_falcon.draw
      @hieros.each(&:draw)
      @obstacles.each(&:draw)
      @enemies.each(&:draw)
    end
  end
end
