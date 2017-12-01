# Responsavel por manipular as boxes de GameObjects
class Box
  attr_accessor :x, :y, :level, :x_offset, :y_offset

  # Construtor da box
  # @param x Coordenada x da tela
  # @param y Coordenada y da tela
  # @param level Nivel de altitude do objeto
  # @param w Largura da imagem do objeto
  # @param h Altura da imagem do objeto
  def initialize(x, y, level, w, h)
    @x = x
    @y = y
    @level = level
    @w = w
    @h = h
  end

  # Inicializa os offsets da box
  # @param x Offset X
  # @param y Offset Y
  def offsets(x, y)
    @x_offset = x
    @y_offset = y
  end

  # Verifica colisoes entre duas Box
  # @param other Outra box a ser analisada
  # @param id Tipo de objeto ao qual a box pertence
  # @return True caso haja alguma sobreposicao entre as duas boxes.
  def overlaps_with(other, id)
    if @level != other.level
      # Caso estejam em alturas(level) diferentes, nao existe colisao
      # Do nothing
    elsif id == Id::HIERO
      # Caso a distancia seja pequena o suficiente, ha colisao
      return Gosu.distance(@x, @y, other.x, other.y) < 25
    elsif id == Id::OBSTACLE
      # Caso a distancia seja pequena o suficiente, ha colisao
      return Gosu.distance(@x, @y, other.x, other.y) < 56
    else
      # Caso a distancia seja pequena o suficiente, ha colisao
      return Gosu.distance(@x, @y, other.x, other.y) < 29
    end
    false
  end
end
