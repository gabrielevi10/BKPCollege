require_relative '../lib/game-object/game_object.rb'
require_relative '../lib/game-ui/window.rb'
require 'gosu'

RSpec.describe 'Classe Falcon' do
  before(:each) do
    @falcao_test = Falcon.new(30, 300, 1)
  end

  context 'Verifica inicialização de variáveis membros' do
    it 'Velocidades definidas corretamente' do
      expect(@falcao_test.speedx).to eq(4)
      expect(@falcao_test.speedy).to eq(2)
    end

    it 'Coordenadas x, y e z inicializadas corretamente' do
      expect(@falcao_test.box.x).to eq(30)
      expect(@falcao_test.box.y).to eq(300)
      expect(@falcao_test.box.level).to eq(1)
    end
  end

  context 'Verifica corretude da movimentação' do
    it 'Move para cima' do
      @falcao_test.move_up
      expect(@falcao_test.box.level).to eq(2)
    end

    it 'Move para baixo' do
      @falcao_test.move_down
      expect(@falcao_test.box.level).to eq(0)
    end

    it 'Move para direita' do
      @falcao_test.move_right
      expect(@falcao_test.box.x).to eq(34)
      expect(@falcao_test.box.y).to eq(302)
    end

    it 'Move para esquerda' do
      @falcao_test.move_left
      expect(@falcao_test.box.x).to eq(26)
      expect(@falcao_test.box.y).to eq(298)
    end
  end

  context 'Verifica corretude do sprite' do
    it 'Tileset carregado corretamente' do
      expected_tiles = Gosu::Image.load_tiles('assets/image/falcon.png', 32, 23)
      falcon_tiles = @falcao_test.sprite.animation

      expected_tiles.zip(falcon_tiles).each do |expected, falcon|
        expect(expected.to_blob).to eq(falcon.to_blob)
      end
    end
  end
end

RSpec.describe 'Classe Hiero' do
  before(:each) do
    @hiero_test = Hiero.new(30, 300, 0)
  end

  context 'Verifica inicialização de variáveis membros' do
    it 'Posição inicializada é válida' do
      expect(@hiero_test.valid).to be true
    end

    it 'Hiero é atualizado corretamente ' do
      @hiero_test.update
      expect(@hiero_test.box.x).to eq(28)
      expect(@hiero_test.box.y).to eq(302)
    end
  end

  context 'Verifica corretude do sprite' do
    it 'Tileset carregado corretamente' do
      expected_tiles = Gosu::Image.load_tiles('assets/image/hiero.png', 16, 16)
      hiero_tiles = @hiero_test.sprite.animation

      expected_tiles.zip(hiero_tiles).each do |expected, hiero|
        expect(expected.to_blob).to eq(hiero.to_blob)
      end
    end
  end
end

RSpec.describe 'Classe Obstacle' do
  before(:each) do
    @obstacle_test = Obstacle.new(30, 300, 0)
  end

  it 'Posicao iniciada valida' do
    expect(@obstacle_test.valid).to be true
  end

  it 'Obstacle é atualizado corretamente' do
    @obstacle_test.update
    expect(@obstacle_test.box.x).to eq(28)
    expect(@obstacle_test.box.y).to eq(302)
  end

  context 'Verifica corretude do sprite' do
    it 'Tileset carregado corretamente' do
      correct_tiles = Gosu::Image.load_tiles('assets/image/piramid.png', 87, 58)
      obstacle_tiles = @obstacle_test.sprite.animation

      correct_tiles.zip(obstacle_tiles).each do |expected, obstacle|
        expect(expected.to_blob).to eq(obstacle.to_blob)
      end
    end
  end
end

RSpec.describe 'Classe Enemy' do
  before(:each) do
    @enemy_test = Enemy.new(30, 300, 0)
  end

  it 'Posicao iniciada valida' do
    expect(@enemy_test.valid).to be true
  end

  it 'Enemy é atualizado corretamente' do
    @enemy_test.update
    expect(@enemy_test.box.x).to eq(27)
    expect(@enemy_test.box.y).to eq(303)
  end

  context 'Verifica corretude do sprite' do
    it 'Tileset carregado corretamente' do
      expected_tiles = Gosu::Image.load_tiles('assets/image/enemy.png', 29, 29)
      enemy_tiles = @enemy_test.sprite.animation

      expected_tiles.zip(enemy_tiles).each do |expected, enemy|
        expect(expected.to_blob).to eq(enemy.to_blob)
      end
    end
  end
end

RSpec.describe 'Teste de colisão entre Falcon e qualquer outro objeto' do
  before(:each) do
    @falcao_test = Falcon.new(30, 300, 1)
  end

  it 'Dois objetos sobrepostos no mesmo nível colidem' do

    hiero_test = Hiero.new(30, 300, 1)
    obt_test = Obstacle.new(30, 300, 1)
    enemy_test = Enemy.new(30, 300, 1)
    expect(@falcao_test.notify_collision(hiero_test, Id::HIERO)).to eq(true)
    expect(@falcao_test.notify_collision(obt_test, Id::OBSTACLE)).to eq(true)
    expect(@falcao_test.notify_collision(enemy_test, Id::ENEMY)).to eq(true)
  end

  it 'Dois objetos parcialmente sobrepostos no mesmo nível colidem' do
    hiero_test = Hiero.new(35, 305, 1)
    obt_test = Obstacle.new(35, 305, 1)
    enemy_test = Enemy.new(35, 305, 1)
    expect(@falcao_test.notify_collision(hiero_test, Id::HIERO)).to eq(true)
    expect(@falcao_test.notify_collision(obt_test, Id::OBSTACLE)).to eq(true)
    expect(@falcao_test.notify_collision(enemy_test, Id::ENEMY)).to eq(true)
  end

  it 'Dois objetos não sobrepostos no mesmo nível não colidem' do
    hiero_test = Hiero.new(0, 480, 1)
    obt_test = Obstacle.new(0, 480, 1)
    enemy_test = Enemy.new(0, 480, 1)
    expect(@falcao_test.notify_collision(hiero_test, Id::HIERO)).to eq(false)
    expect(@falcao_test.notify_collision(obt_test, Id::OBSTACLE)).to eq(false)
    expect(@falcao_test.notify_collision(enemy_test, Id::ENEMY)).to eq(false)
  end

  it 'Dois objetos em níveis diferentes não colidem' do    
    hiero_test = Hiero.new(30, 300, 0)
    obt_test = Obstacle.new(30, 300, 0)
    enemy_test = Enemy.new(30, 300, 0)    
    expect(@falcao_test.notify_collision(hiero_test, Id::HIERO)).to eq(false)
    expect(@falcao_test.notify_collision(obt_test, Id::OBSTACLE)).to eq(false)
    expect(@falcao_test.notify_collision(enemy_test, Id::ENEMY)).to eq(false)
  end
end

RSpec.describe 'Teste de escolha no menu' do
  before(:each) do
    @window_test = Window.new(1440, 900)
  end

  it 'Caso seja pressionado 1' do
    @window_test.button_down(Gosu::Kb1)
    expect(@window_test.stage).to eq(Stages::GAME)
  end

  it 'Caso seja pressionado 2' do
    @window_test.button_down(Gosu::Kb2)
    expect(@window_test.stage).to eq(Stages::SCOREBOARD)
  end

  it 'Caso seja pressionado 3' do
    @window_test.stage = Stages::GAME
    @window_test.button_down(Gosu::Kb3)
    expect(@window_test.stage).to eq(Stages::MENU)
  end
end
