i18n = new VueI18n({
  locale: navigator.language, // 设置地区
  fallbackLocale: 'en',
  messages: i18n_messages, // 设置地区信息
})
vm = __chat_vue_vm = new Vue({
  i18n,
  el: '#chat-container',
  data: () => {
    return {
      messageList: [
        // {
        //   uuid: '2',
        //   context:
        //     '111我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理',
        //   user: 'assistant',
        //   time: '11:12',
        //   done: false,
        //   html: '<p>11我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理我是智能助理</p><p>我是智能助理</p>',
        // },
        // {
        //   uuid: '1',
        //   context:
        //     'Happiness is a state of mind. It starts with accepting where you are, knowing where you are going and planning to enjoy every moment along the way. You know how to be happy, and feel that you have enough time or money or love or whatever you need to achieve your goals. And just feeling that you have enough of everything means that you do indeed have enough.',
        //   user: 'user',
        //   time: '11:12',
        // },
        // {
        //   uuid: '3',
        //   context: '我是智能助理\n我是智能助理\n**你好**',
        //   user: 'assistant',
        //   time: '11:12',
        //   done: true,
        //   html: '2024年8月26日15:17:32<p>You have to choose to be happy, and focus upon being happy, in order to be happy. If you instead focus upon knowing that you will be happy if you achieve something, you will never be happy, as you have not learned to “smell the roses”. The irony is that when you are happy, you are inevitably more productive, and far more likely to achieve what everything-seekers are seeking.</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p>',
        // },
        // {
        //   uuid: '1',
        //   context: '说点话试试',
        //   user: 'user',
        //   time: '11:12',
        // },
        // {
        //   uuid: '4',
        //   context: '我是智能助理\n我是智能助理\n**你好**',
        //   user: 'assistant',
        //   time: '11:12',
        //   done: false,
        //   html: '<p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p><p>我是智能助理</p>',
        // },
        // {
        //   uuid: '5',
        //   context: '',
        //   user: 'assistant',
        //   time: '11:12',
        //   done: false,
        //   html: '',
        // },
      ],
    }
  },
  methods: {
    addMessage(message) {
      console.log(message)
      // 填充时间
      if (!message.time) {
        message.time = getNowTime()
      }
      // 置空html
      message.html = ''
      // 通过message的uuid判断这个msg是否已经存在
      var msgIndex = this.messageList.findIndex(
        (msg) => msg.uuid == message.uuid
      )

      // 合并内容
      var oldContext = msgIndex == -1 ? '' : this.messageList[msgIndex].context
      message.context = oldContext + message.context
      // 解析markdown
      if (message.context) {
        message.html = formateMarkdown(message.context)
      }

      if (msgIndex == -1) {
        // 新的，添加进去即可
        this.messageList.push(message)
      } else {
        // 旧的，更新一下
        this.messageList.splice(msgIndex, 1, message)
      }

      setTimeout(() => {
        window.scrollTo(0, document.body.scrollHeight)
        // scrollToBottom()
      })

      this.$nextTick(() => {
        // 拦截a标签
        interceptLink()
      })
    },
    // 处理机器人的消息被点击
    handleBotMessageClick(e, message) {
      if (e.target && e.target.tagName === 'A') {
        // 捕获点击事件并处理
        __qt_core.on_toQtJsonObject({
          type: 'link',
          data: e.target.href,
          uuid: message.uuid,
        })
      }
    },
    // 清空消息上下文
    clearMessageContext() {
      console.log('clearMessageContext')
      this.messageList = []
      __qt_core.on_clearContext()
    },
  },
  created() {
    console.log('create')
  },
  mounted() {
    console.log('mounted')
    interceptLink()
    this.$nextTick(() => {
      window.scrollTo(0, document.body.scrollHeight)
    })
  },
})

/**
 * qt给web发消息
 * @param {} msg
 */
function showText(msg) {
  var vm = __chat_vue_vm
  vm.addMessage(msg)
}

/**
 * 获取格式化之后的时间
 * @returns
 */
function getNowTime() {
  var currentTime = new Date()
  var hours = currentTime.getHours()
  var minutes = currentTime.getMinutes()

  if (hours < 10) {
    hours = '0' + hours
  }
  if (minutes < 10) {
    minutes = '0' + minutes
  }
  var formattedTime = hours + ':' + minutes
  return formattedTime
}

/**
 * 解析markdown
 * @param {} message
 */
function formateMarkdown(message) {
  var renderer = new marked.Renderer()
  renderer.code = function (code, language) {
    var highlightedCode = hljs.highlightAuto(code).value
    return (
      '<pre><code class="hljs ' +
      language +
      '">' +
      highlightedCode +
      '</code></pre>'
    )
  }

  marked.setOptions({
    renderer: renderer,
    gfm: true,
    tables: true,
    breaks: true,
    pedantic: false,
    sanitize: false,
    smartLists: true,
    smartypants: false,
  })

  return marked.parse(message)
}

/**
 * 滚动到底部
 */
function scrollToBottom() {
  document.getElementById('bottom').scrollIntoView({ behavior: 'smooth' })
  // window.scrollTo(0, document.documentElement.scrollHeight)
}

/**
 * 捕获a标签的点击事件
 */
function interceptLink() {
  document.querySelectorAll('a').forEach((a) => {
    a.onclick = (event) => {
      event.preventDefault() // 阻止默认跳转行为
      return false
    }
  })
}

// window.addEventListener('load', () => {
//   let refreshCount = 0 // 初始化计数器
//   console.log('初始化计数器')

//   document.addEventListener('keydown', function (event) {
//     // 检查按下的是'R'键（不分大小写）
//     if (event.key.toLowerCase() === 'r') {
//       refreshCount++ // 增加计数器
//       console.log('按下了 R 键，当前计数:', refreshCount)

//       if (refreshCount === 3) {
//         console.log('已按三次R，即将刷新页面...')
//         location.reload() // 刷新页面
//         refreshCount = 0 // 重置计数器
//       }
//     }
//   })
// })
