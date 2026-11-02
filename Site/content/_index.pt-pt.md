---
title: "Shprot — proxy SOCKS5 através de SSH que volta a ligar-se sozinho"
description: "Aplicação gratuita para Windows e Linux que mantém um proxy SOCKS5 e HTTP através de SSH a funcionar: volta a ligar-se após mudanças de rede e suspensão, deteta túneis bloqueados, sem linha de comandos."
readme: "README.md"

ui:
  skip: "Saltar para o conteúdo"
  nav_features: "Funcionalidades"
  nav_compare: "Porquê o Shprot"
  nav_download: "Transferir"
  nav_faq: "Perguntas"
  language: "Idioma"
  github: "Código-fonte no GitHub"

hero:
  title: "Um proxy SOCKS5 através de SSH que se mantém ligado"
  lead: "O Shprot transforma qualquer servidor a que acede por SSH num proxy SOCKS5 e HTTP estável. Vigia a sua rede, restabelece o túnel depois de mudar de Wi-Fi ou de sair da suspensão e diz-lhe exatamente o que correu mal — sem linha de comandos."
  download: "Transferir"
  download_hint: "Versão {version} para Windows e Linux"
  facts:
    - "Gratuito e de código aberto"
    - "Windows 10 / 11 e Linux"
    - "49 idiomas"
  screenshot_alt: "Janela de definições do Shprot"

features:
  title: "Tudo o que um túnel precisa para continuar a funcionar"
  items:
    - id: reconnect
      title: "Resiste a mudanças de rede"
      text: "Mudou de Wi-Fi, ligou um cabo ou acordou o portátil? O Shprot deteta-o e restabelece o túnel sozinho."
    - id: health
      title: "Apanha falhas silenciosas"
      text: "Verificações regulares através do próprio proxy detetam um túnel que parece ligado mas não deixa passar tráfego, e reiniciam-no."
    - id: gentle
      title: "Poupa o seu servidor"
      text: "As novas tentativas vão ficando mais espaçadas e, após falhas de autenticação, repetem-se no máximo uma vez por minuto, para que o fail2ban não bloqueie o seu IP por causa de uma chave errada."
    - id: keys
      title: "Chaves SSH em dois cliques"
      text: "Gere ou importe uma chave e copie a parte pública para o `authorized_keys` diretamente na janela de definições."
    - id: trust
      title: "Verificação clara do servidor"
      text: "Na primeira ligação, a impressão digital da chave do servidor é mostrada numa caixa de diálogo, para que a possa verificar antes de confiar no servidor."
    - id: http
      title: "Também proxy HTTP"
      text: "As aplicações sem suporte para SOCKS5 podem usar um proxy HTTP opcional que passa pelo mesmo túnel."
    - id: tray
      title: "Vive na área de notificação"
      text: "O ícone mostra se o túnel está ativo, as notificações explicam porque não está e os registos estão à distância de um clique."
    - id: languages
      title: "Fala a sua língua"
      text: "A interface está traduzida para 49 idiomas, com disposição da direita para a esquerda para árabe, persa, hebraico e urdu."

compare:
  title: "Porque não usar apenas `ssh -D`?"
  lead: "`ssh -D 1080 user@server` dá-lhe um proxy SOCKS5 com um único comando — até a ligação deixar de funcionar sem aviso. O Shprot executa o mesmo cliente OpenSSH e trata de tudo o que o comando sozinho não faz."
  rows:
    - case: "Mudança de rede ou de Wi-Fi, saída da suspensão"
      ssh: "Reinicie-o manualmente"
      shprot: "Volta a ligar-se automaticamente"
    - case: "Túnel ligado mas sem tráfego"
      ssh: "Passa despercebido"
      shprot: "Detetado por verificações através do proxy"
    - case: "Falhas de autenticação repetidas"
      ssh: "Fica a cargo dos seus scripts"
      shprot: "Tentativas espaçadas para que o servidor não bloqueie o seu IP"
    - case: "Chaves do cliente"
      ssh: "`ssh-keygen` e ficheiros em `~/.ssh`"
      shprot: "Gerar ou importar, copiar a chave pública com um clique"
    - case: "Verificação da identidade do servidor"
      ssh: "Pergunta no terminal"
      shprot: "Caixa de diálogo com a impressão digital da chave"
    - case: "Aplicações sem suporte para SOCKS5"
      ssh: "Sem solução"
      shprot: "Proxy HTTP opcional pelo mesmo túnel"
    - case: "Estado do túnel"
      ssh: "Janela do terminal"
      shprot: "Ícone na área de notificação e notificações com o motivo da falha"
  note: "Ferramentas como o `autossh` ajudam nos reinícios, mas continuam a exigir configuração manual e não têm interface."

uses:
  title: "Para que serve"
  items:
    - id: infra
      title: "A sua própria infraestrutura"
      text: "Aceda a painéis internos, ambientes de testes e ferramentas de administração disponíveis apenas na rede do seu servidor."
    - id: wifi
      title: "Redes pouco fiáveis"
      text: "Envie o seu tráfego pelo seu próprio servidor quando usa Wi-Fi público em cafés, hotéis ou aeroportos."
    - id: laptop
      title: "Trabalho remoto no portátil"
      text: "Mantenha um proxy estável num computador que suspende e muda de rede durante todo o dia."

how:
  title: "Pronto em três passos"
  steps:
    - title: "Prepare um servidor"
      text: "Serve qualquer servidor Linux com OpenSSH. Não é instalado nada: um utilizador dedicado, que só pode reencaminhar portas, cria-se com poucos comandos."
    - title: "Ligue o Shprot"
      text: "Introduza `user@server`, gere uma chave e adicione a parte pública ao `authorized_keys` do servidor. Verifique uma vez a impressão digital do servidor."
    - title: "Aponte as aplicações para o proxy"
      text: "Use `socks5h://localhost:1080` no navegador ou nas definições de proxy do sistema — o menu do ícone copia o endereço por si."
  guide: "Guia passo a passo com proteção do servidor"

tour:
  title: "Mais de perto"
  items:
    - id: trust
      caption: "Na primeira ligação vê a impressão digital da chave do servidor e decide se confia nele."
    - id: tray
      caption: "O menu do ícone mostra o estado do túnel e copia endereços de proxy prontos a usar."

download:
  title: "Transferir o Shprot"
  lead: "Versão {version}. Gratuito, de código aberto, sem contas nem subscrições."
  windows:
    title: "Windows"
    text: "Windows 10 e 11, 64 bits. Para atualizar, instale a nova versão por cima da antiga."
    button: "Instalador (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ e distribuições compatíveis."
    button: "Pacote (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ e distribuições compatíveis."
    button: "Pacote (.rpm)"
  requirements: "O Shprot usa o cliente OpenSSH (`ssh` e `ssh-keygen`). Vem incluído no Windows 10 / 11 e na maioria dos ambientes de trabalho Linux; se faltar, o Shprot explica como o adicionar."
  verify: "Cada versão inclui um ficheiro `SHA256SUMS` para verificar que a transferência não foi alterada."
  all_releases: "Todas as versões e alterações"

faq:
  title: "Perguntas e respostas"
  items:
    - q: "O Shprot é uma VPN?"
      a: "Não, é um proxy. Só as aplicações configuradas para o usar — um navegador, `curl`, `git`, um IDE — enviam tráfego pelo túnel; tudo o resto segue diretamente. Não são instalados adaptadores de rede virtuais."
    - q: "De que preciso no servidor?"
      a: "De um servidor OpenSSH normal e de uma conta autorizada a reencaminhar portas TCP. Não é preciso instalar nada no servidor; serve qualquer VPS ou servidor doméstico acessível por SSH."
    - q: "Posso iniciar sessão com uma palavra-passe?"
      a: "Não, o Shprot usa apenas chaves SSH. As chaves são mais seguras do que as palavras-passe e permitem que o túnel volte a ligar-se sem intervenção. A chave é gerada na aplicação com um clique."
    - q: "Que aplicações podem usar o proxy?"
      a: "Navegadores, `curl`, `git`, gestores de pacotes e qualquer aplicação com definições de proxy SOCKS5 ou HTTP ou com suporte para as variáveis `ALL_PROXY` / `HTTP_PROXY`. Use `socks5h://` para que os nomes dos sites sejam resolvidos no servidor."
    - q: "Como é que o Shprot verifica o túnel?"
      a: "De vez em quando abre, através do proxy, um site popular escolhido ao acaso. Se falhar enquanto a internet está disponível, o túnel é reiniciado."
    - q: "É gratuito?"
      a: "Sim. O Shprot é software livre e de código aberto sob a licença GNU GPL v3, sem contas, subscrições nem publicidade."
    - q: "Existe uma versão para macOS?"
      a: "Ainda não. Atualmente o Shprot suporta Windows 10 / 11 e Linux."

footer:
  license: "O Shprot é software livre licenciado sob a GNU GPL v3."
  source: "Código-fonte"
  issues: "Comunicar um problema"
---
