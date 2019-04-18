<template>
  <div id="app">
    <div class="title_css">
    <x-header :left-options="{showBack: false}" style="background-color:red">想想</x-header>
    </div>  
     <swiper atuo="true" interval=2000 :list="demo01_list" v-model="demo01_index" @on-index-change="demo01_onIndexChange" > 
     </swiper>
       <p class="copyright">Image Source: http://www.gratisography.com/</p>
       <p>current index: {{demo01_index}}</p>
    <x-button @click.native="demo01_index = 0">go to 0</x-button>
    <x-button @click.native="demo01_index = 1">go to 1</x-button>
    <x-button @click.native="demo01_index = 2">go to 2</x-button>
       <swiper :auto="true" height="100px" direction="vertical" :interval=2000 class="text-scroll" :show-dots="false">
      <swiper-item><p>义务爱了 完成传奇世界H5-王者归来任务 获得20金币</p></swiper-item>
      <swiper-item><p>基本世神 兑换《传奇世界H5》畅玩级礼包 消耗30金币</p></swiper-item>
      <swiper-item><p>零哥章魚 完成传奇世界H5-王者归来任务 获得30金币</p></swiper-item>
       <br>
       <swiper :auto="true" height="100px" direction="vertical" :interval=2000 class="text-scroll" :show-dots="false">
      <swiper-item><p>做迎而為 兑换【饿了么】畅享美食红包 消耗20金币</p></swiper-item>
      <swiper-item><p>只知道不知道 兑换【饿了么】畅享美食红包 消耗20金币</p></swiper-item>
      <swiper-item><p>基本世神 兑换《传奇世界H5》畅玩级礼包 消耗30金币</p></swiper-item>
     
    </swiper>
       </swiper>
    <router-view/>
  </div>
</template>

<script>
const baseList = [{
  url: 'javascript:',
  img: '../assets/2.png',
  title: '送你一朵fua'
}, {
  url: 'javascript:',
  img: '../assets/2.png',
  title: '送你一辆车'
}, {
  url: 'javascript:',
  img: "../assets/2.png", // 404
  title: '送你一次旅行',
  fallbackImg: 'https://ww1.sinaimg.cn/large/663d3650gy1fq66vw50iwj20ff0aaaci.jpg'
}]
export default {
  name: 'App',
   ready () {
  },
  methods: {
    demo01_onIndexChange (index) {
      this.demo01_index = index
    }
    
  },
  data () {
    return {
      demo01_list: baseList,
       demo01_index: 0,
        swiperItemIndex: 1
    }
}
}
</script>

<style lang="less">
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color:red;
  margin-top: -8px;
}
.title_css{
margin-top: 8px;
}
</style>
