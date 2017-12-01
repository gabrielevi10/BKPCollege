# Responsavel por manipular sprites
class Sprite
  attr_reader :animation

  # Opacidade da sombra definida de acordo com a coordenada Z do objeto.
  SHADOW_LEVEL = [0, 0xff_000000, 0xaa_000000, 0x55_000000].freeze

  # Carrega o sprite com os tiles
  # @param filename Nome do arquivo
  # @param x Largura de cada tile
  # @param y Altura de cada tile
  def initialize(filename, x, y)
    # Carrega as animacoes
    @animation = Gosu::Image.load_tiles(filename, x, y)
    @font = Gosu::Font.new(20)
  end

  # Renderiza o sprite
  # @param box Informacoes da posicao do objeto a ser renderizado
  # @param object_id Tipo de objeto a ser renderizado
  # @param score Pontuacao a ser renderizada caso seja um Falcao a ser desenhado
  def draw(box, object_id, score = 0)
    x = box.x + box.x_offset

    # Define qual frame sera desenhado
    @current_image = @animation[Gosu.milliseconds / 125 % @animation.size]
    @current_image.draw(x, box.y - box.level * 20, box.level + 1, 1.0, 1.0)

    # Renderiza a pontuacao caso seja o Falcao sendo renderizado
    if object_id == Id::FALCON
      @font.draw("Score: #{score}", 10, 10, 1, 1.0, 1.0, Gosu::Color::BLACK)
    end

    # Renderiza a sombra do objeto
    @current_image.draw(x, box.y, 1.0, 1.0, 1.0, SHADOW_LEVEL[box.level])
  end
end
