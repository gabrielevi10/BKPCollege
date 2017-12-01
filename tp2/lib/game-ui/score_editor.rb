# Editor das pontuacoes do ranking, incluindo criar e deletar
# o arquivo texto assim como manipula-lo.
module ScoreEditor
  # Numero maximo de membros no ranking
  SCORE_MAX_SIZE = 10
  attr_accessor :scores

  # Hash com chave sendo o nome de usuario e valor sua maior pontuacao
  @scores = Hash.new(0)

  # Le o arquivo com o ranking atual
  def read
    File.open('assets/text/score.txt', 'r+') do |infile|
      while (line = infile.gets)
        # Le as linhas e as coloca no hash
        arr = line.split(' ')
        # Transforma a string que representa a pontuacao em int
        arr[1] = arr[1].to_i
        # E enfim adiciona ao hash
        @scores[arr[0]] = arr[1]
      end
    end
  end

  # Ordena as pontuacoes carregadas na memoria
  # @return Array de pares string-inteiro com username-pontuacao
  def sort
    # Ordena o placar decrescentemente
    sorted = @scores.sort_by { |_k, v| v }.reverse
    # E retorna-o
    sorted
  end

  # Grava todas pontuacoes salvas na memoria em um arquivo separado
  # @param sorted Vetor com pares string-inteiro com username-pontuacao ordenado
  #               de forma decrescente comparando as pontuacoes.
  def write(sorted)
    this_file = File.open('assets/text/score.txt', 'w+')
    loop_size = sorted.size < SCORE_MAX_SIZE ? sorted.size : SCORE_MAX_SIZE

    (0...loop_size).each do |i|
      # Escreve nome
      this_file.write(sorted[i][0])
      this_file.write(' ')
      # Escreve score
      this_file.write(sorted[i][1])
      this_file.write("\n")
    end
    this_file.close
  end

  # Atualiza a pontuacao do usuario no ranking para sua maior ja feita.
  # @param name Nome de usuario
  # @param this_score Pontuacao do usuario
  def add(name, this_score)
    @scores[name] = [this_score, @scores[name]].max
  end
end
