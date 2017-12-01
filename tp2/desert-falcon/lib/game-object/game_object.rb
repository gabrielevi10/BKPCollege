require_relative 'box.rb'
require_relative 'sprite.rb'

# Numero identificador de cada tipo de objeto
module Id
  # ID de um objeto do tipo Falcon
  FALCON    = 1
  # ID de um objeto do tipo Hiero
  HIERO     = 2
  # ID de um objeto do tipo Obstacle
  OBSTACLE  = 3
  # ID de um objeto do tipo Enemy
  ENEMY     = 4
end

# Classe base virtual de um objeto do jogo.
class GameObject
  # Coordenada Z (level) maxima de um objeto
  MAX_LEVEL = 3

  # Coordenada Z (level) minima de um objeto
  MIN_LEVEL = 0

  attr_accessor :is_dead, :box, :sprite

  # Construtor da classe
  def initialize(_x, _y, _level)
    @is_dead = false
  end

  # Renderiza o sprite nas coordenadas da box
  def draw
    @sprite.draw(@box)
  end

  # Atualiza o game object
  def update; end

  # Verifica se outro objeto eh igual a este
  # @param other Outro objeto
  def equal?(other); end

  # Verifica se o objeto esta morto
  # @return True se o objeto estiver morto
  def dead?
    @is_dead
  end

  # Verifica se dois objetos estao colidindo
  # @param other Outro objeto de jogo
  # @param id Identificador do tipo de objeto a ser comparado
  def notify_collision(other, id)
    @box.overlaps_with(other.box, id)
  end
end

# Classe com metodos que definem o comportamento do Falcon
class Falcon < GameObject
  attr_accessor :speedx, :speedy, :score

  # Construtor
  # @param x Posicao X inicial
  # @param y Posicao Y inicial
  # @param level Posicao Z (level) inicial
  def initialize(x, y, level)
    super
    @sprite = Sprite.new('assets/image/falcon.png', 32, 23)
    @box = Box.new(x, y, level, 32, 23)
    @box.offsets(16, 11)

    @width = 640
    @height = 480

    # ground e sky sao os limites das coordenadas x e y
    @ground = @height
    @sky = 40

    @speedx = 4
    @speedy = 2
    # level eh a coordenada z do falcao, diz respeito ao quao alto este voa
    @level = level
    @score = 0
  end

  # Desenha o falcao e sua sombra
  def draw
    @sprite.draw(@box, Id::FALCON, @score)
  end

  # Move para direita ou esquerda de acordo com a seta apertada
  def update
    move_left  if Gosu.button_down?(Gosu::KbLeft)
    move_right if Gosu.button_down?(Gosu::KbRight)
  end

  # Controla os movimentos cima e baixo do falcao
  def button_down(id)
    # Como requerem apenas uma atualizacao quando pressionado o botao
    # foram deslocadas para esta funcao.
    move_up   if id == Gosu::KbUp
    move_down if id == Gosu::KbDown
  end

  # Move o falcao para a direcao noroeste
  def move_left
    # Caso o falcao esteja dentre os limites possiveis, execute
    # sky controla o limite da coordenada y.
    return unless @box.x > 0 && @box.y > @sky
    @box.x -= @speedx
    @box.y -= @speedy
  end

  # Move o falcao para a direcao sudeste
  def move_right
    # Caso o falcao esteja dentre os limites possiveis, execute
    # "ground" controla o limite da coordenada 'x'.
    # Eh usado o offset para controlar se a imagem do falcao
    # esta em limites validos.
    return unless @box.x < (@width - (@box.x_offset * 3 + 10))
    return unless (@box.y + 40) < @ground
    @box.x += @speedx
    @box.y += @speedy
  end

  # Sobe o nivel do falcao
  def move_up
    # Caso nao tenha atingido o nivel maximo eh possivel subir
    @box.level += 1 unless @box.level >= MAX_LEVEL
  end

  # Diminui o nivel do falcao
  def move_down
    # Caso nao tenha atingido o nivel minimo eh possivel subir
    @box.level -= 1 unless @box.level <= MIN_LEVEL
  end

  # Verifica se houve colisao
  def notify_collision(other, id)
    super
  end
end

# Classes com metodos que definem o comportamento de um Hiero
class Hiero < GameObject
  # Altura do sprite do Hiero
  HEIGHT = 16
  # Largura do sprite do Hiero
  WIDTH = 16
  # Velocidade que o Hiero atravessa a tela
  SPEED = 2

  # Construtor
  # @param x Posicao X inicial
  # @param y Posicao Y inicial
  # @param level Posicao Z (level) inicial
  def initialize(x, y, level)
    super
    @sprite = Sprite.new('assets/image/hiero.png', WIDTH, HEIGHT)
    @box = Box.new(x, y, level, WIDTH, HEIGHT)
    @box.offsets(8, 8)
  end

  # Atualiza as coordenadas do Hiero
  def update
    @box.x -= SPEED
    @box.y += SPEED
  end

  # Verifica se o Hiero esta dentro dos limites da tela
  # @return True caso o Hiero esteja em uma posicao valida
  def valid
    @box.x > (0 - WIDTH)
  end

  # Renderiza o sprite do Hiero
  def draw
    @sprite.draw(@box, Id::HIERO)
  end

  # Verifica se houve colisao com outro objeto
  def notify_collision(other, id)
    super
  end
end

# Classe que define o comportamento de um obstaculo (piramide)
class Obstacle < GameObject
  # Altura do sprite do Obstacle
  HEIGHT = 58
  # Largura do sprite do Obstacle
  WIDTH = 87
  # Velocidade na qual a Piramide atravessa a tela
  SPEED = 2

  attr_accessor :level

  # Construtor
  # @param x Posicao X inicial
  # @param y Posicao Y inicial
  # @param level Posicao Z (level) inicial
  def initialize(x, y, level)
    super
    @sprite = Sprite.new('assets/image/piramid.png', WIDTH, HEIGHT)
    @box = Box.new(x, y, level, WIDTH, HEIGHT)
    @box.offsets(0, 0)
    @level = level
  end

  # Atualiza a posicao da piramide constantemente
  def update
    @box.x -= SPEED
    @box.y += SPEED
  end

  # Retorna a validez da posicao da piramide
  # @return True se a piramide estiver dentro dos limites da tela
  def valid
    @box.x > (0 - WIDTH)
  end

  # Renderiza o sprite da Piramide
  def draw
    @sprite.draw(@box, Id::OBSTACLE)
  end

  # Verifica se houve colisao
  def notify_collision(other, id)
    super
  end
end

# Classe que define o comportamento de um Inimigo
class Enemy < GameObject
  # Altura do sprite do Enemy
  HEIGHT = 29
  # Largura do sprite do Enemy
  WIDTH = 29
  # Velocidade que um inimigo atravessa a tela
  SPEED = 3

  # Construtor
  # @param x Posicao X inicial
  # @param y Posicao Y inicial
  # @param level Posicao Z (level) inicial
  def initialize(x, y, level)
    super
    @sprite = Sprite.new('assets/image/enemy.png', WIDTH, HEIGHT)
    @box = Box.new(x, y, level, WIDTH, HEIGHT)
    @box.offsets(8, 8)
    @level = level
  end

  # Atualiza as coordenadas do inimigo constantemente
  def update
    @box.x -= SPEED
    @box.y += SPEED
  end

  # Verifica se o Inimigo esta em uma posicao valida
  # @return True se ele estiver dentro dos limites da tela
  def valid
    @box.x > (0 - WIDTH)
  end

  # Renderiza um inimigo e sua sombra
  def draw
    @sprite.draw(@box, Id::ENEMY)
  end

  # Verifica se houve colisao
  def notify_collision(other, id)
    super
  end
end
