# 하모니카 런처

하모니카에서 사용하는 런처 프로젝트

단축키 Alt+Space 키를 눌러 PC에 설치된 프로그램과 웹에서 바로 검색할 수 있습니다.

![screenshot](./albert.png)


upstream : https://github.com/albertlauncher/albert.git


# Install

## HamoniKR (>=3.0), Ubuntu (>=18.04)
```
wget -qO- https://pkg.hamonikr.org/add-hamonikr.apt | sudo -E bash -

sudo apt install albert
```

# Support

* [HamoniKR Community](https://hamonikr.org/)


# gpt plugin

![gpt_plugin](./gpt_plugin.png)

* OpenAI API 키를 설정하면 GTP 와 채팅을 할 수 있는 플러그인
* 설정 파일 `~/.config/albert/albert.conf` 의 내용 중 아래 부분을 자신이 사용 가능한 API 키로 변경

openai_api_key=YOUR_OPENAI_API_KEY

```
openai_api_key=OpenAI 에서 발급 받은 API 키

```
이미지와 같이 입력창에 ai> 프롬프트 를 입력하면 응답이 아래에 보인다.

