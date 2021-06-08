import bambo from './bambo';

/** @preserve
 * Clock Panel Remote
 *
 * Author : Christux
 * Version : 1.0
 * Date : 13 nov 2020
 */

/*
* Navigation module
*/
export default bambo.module('navigate', ['$location', '$scroll', ($location, $scroll) => {

  /*
  * Navigates between pages by handling section visibility and navbar
  */
  let buttonList = [];
  let categoryList = [];

  /*
    * Listen to change anchor event
    */
  $location.registerHandler(displaySelectedCategory);

  return {
    $init: function () {

      buttonList = window.document.getElementsByTagName("header")[0].getElementsByClassName("button");
      categoryList = window.document.getElementById("content").getElementsByClassName("category");

      displaySelectedCategory($location.get());
    }
  };

  //------------------------------------------------------------------------

  /*
  * Sets current page on navbar
  */
  function disactivate(button) {
    button.className = "button active";
  };

  /*
  * Sets other pages on navbar
  */
  function activate(button) {
    button.className = "button unactive";
  };

  /*
  * Displays selected page and update navbar
  */
  function displaySelectedCategory(selectedAnchor) {

    const anchor = selectedAnchor || 'animation';
    const showedCategory = window.document.getElementsByClassName(anchor)[0];

    for (let i in categoryList) {
      if (categoryList[i].style) {

        if (categoryList[i] === showedCategory) {
          categoryList[i].style.display = "block";
          if (buttonList[i]) {
            disactivate(buttonList[i]);
          }
        } else {
          categoryList[i].style.display = "none";
          if (buttonList[i]) {
            activate(buttonList[i]);
          }
        }
      }
    }

    // Goes on the top of the page
    $scroll.reset();
  };
}])

  /*
  * Observer module (GoF design pattern)
  */
  .module('observer', ['$observer', ($observer) => {
    return $observer.create();
  }])

  /*
  * Data model module
  */
  .module('model', ['observer', '$http', (observer, $http) => {

    let url;
    let previous = "";

    setInterval(function () {
      get();
    }, 6000);

    return {
      $init: function () {
        url = '/api';
      },
      $final: function () {
        get();
      },
      update: function () {
        get();
      },
      send: function (data) {
        set(data);
      }
    };

    //--------------------------------------------------
    function get() {

      $http.get(url)
        .setTimeout(10000)
        .success(function (resp) {
          // Little optimization, notifies only if changes
          if (resp !== previous) {
            previous = resp;
            observer.notifyAll(JSON.parse(resp));
          }
        })
        .error(function (resp) {
          console.log(resp);
        })
        .timeout(function (resp) {
          console.log(resp);
        })
        .send();
    }

    function set(obj) {

      $http.post(url, JSON.stringify(obj))
        .setTimeout(10000)
        .success(function (resp) {
          var data = JSON.parse(resp);
          observer.notifyAll(data);
        })
        .error(function (resp) {
          console.log(resp);
        })
        .timeout(function (resp) {
          console.log(resp);
        })
        .send();
    }
  }])

  /*
  * Animation module
  */
  .module('animation', () => {

    let node;
    let buttonList = [];

    const availableModes = [
      "Static color", "Rainbow lamp", "Rainbow",
      "Fire", "Glitter"];

    return {
      $init: ['observer', function (observer) {
        node = document.getElementById("animation");

        observer.registerHandler(function (data) {
          update(data.animation);
        });
      }],
      $build: ['model', function (model) {
        build(model);
      }]
    };

    //-------------------------------------------------------------
    function build(model) {

      for (let i = 0, len = availableModes.length; i < len; i++) {

        const button = document.createElement('div');
        button.innerHTML = availableModes[i];
        button.className = "button unactive";
        button.onclick = (function (I) {
          return function () {
            changeMode(model, I);
          };
        })(i);

        node.appendChild(button);
        buttonList.push(button);
      }
    }

    function changeMode(model, newMode) {
      model.send({
        animation: newMode
      });
    }

    function update(anim) {

      for (let i = 0, len = buttonList.length; i < len; i++) {
        if (anim === i) {
          buttonList[i].className = "button active";
        }
        else {
          buttonList[i].className = "button unactive";
        }
      }
    }
  })

  /*
  * Animation module
  */
  .module('separator', () => {

    let node;
    let buttonList = [];

    const availableModes = [
      "Static color", "Blink", "Breath",
      "Cylon", "Bounce"];

    return {
      $init: ['observer', function (observer) {
        node = document.getElementById("separator");

        observer.registerHandler(function (data) {
          update(data.separator);
        });
      }],
      $build: ['model', function (model) {
        build(model);
      }]
    };

    //-------------------------------------------------------------
    function build(model) {

      for (let i = 0, len = availableModes.length; i < len; i++) {

        const button = document.createElement('div');
        button.innerHTML = availableModes[i];
        button.className = "button unactive";
        button.onclick = (function (I) {
          return function () {
            changeMode(model, I);
          };
        })(i);

        node.appendChild(button);
        buttonList.push(button);
      }
    }

    function changeMode(model, newMode) {
      model.send({
        separator: newMode
      });
    }

    function update(anim) {

      for (let i = 0, len = buttonList.length; i < len; i++) {
        if (anim === i) {
          buttonList[i].className = "button active";
        }
        else {
          buttonList[i].className = "button unactive";
        }
      }
    }
  })

  /*
  * Mirror module
  */
  .module('mirror', () => {

    let node;
    let button;
    let mirror = false;

    return {
      $init: ['observer', function (observer) {
        node = document.getElementById('mirror');

        observer.registerHandler(function (data) {
          update(data.mirror);
        });
      }],
      $build: ['model', function (model) {
        build(model);
      }]
    };

    //-------------------------------------------------------------
    function build(model) {

      button = document.createElement('div');
      button.innerText = 'Mirror';
      button.className = 'button unactive';
      button.onclick = function () {
        changeMode(model, !mirror);
      };

      node.appendChild(button);
    }

    function changeMode(model, newMode) {
      model.send({
        mirror: newMode
      });
    }

    function update(_mirror) {
      mirror = _mirror;

      if (mirror) {
        button.className = "button active";
      }
      else {
        button.className = "button unactive";
      }
    }
  })

  /*
  * Brightness module
  */
  .module('brightness', () => {

    let node;
    let buttonList = [];
    let bColors = [];

    return {
      $init: ['observer', function (observer) {
        node = document.getElementById("brightness");

        observer.registerHandler(function (data) {
          update(data.color);
        });
      }],
      $build: ['model', function (model) {
        build(model);
      }]
    };

    //--------------------------------------------------------------
    function build(model) {

      const tablebody = document.createElement('div');
      tablebody.className = 'divTableBody';
      node.appendChild(tablebody);

      const tablerow = document.createElement('div');
      tablerow.className = 'divTableRow';
      tablebody.appendChild(tablerow);

      for (let i = 0; i < 6; i++) {

        const button = document.createElement('div');
        button.className = "divTableCell";
        button.style.backgroundColor = "black";
        button.onclick = (function (I) {
          return function () {
            changeColor(model, I);
          };
        })(i);

        tablerow.appendChild(button);
        buttonList.push(button);
      }
    }

    function computeColors(color) {

      const r = color.red, g = color.green, b = color.blue;
      const brightness = Math.max(r, g, b, 1); // Avoid dividing by zero
      const brightIdx = Math.max(0, Math.round(6 - brightness / (255 / 6)));

      for (let i = 0; i < 6; i++) {
        const newR = Math.round(r * (1 - 1 / 6 * i) * 255 / brightness);
        const newG = Math.round(g * (1 - 1 / 6 * i) * 255 / brightness);
        const newB = Math.round(b * (1 - 1 / 6 * i) * 255 / brightness);
        bColors[i] = "rgb(" + newR.toString() + "," + newG.toString() + "," + newB.toString() + ")";
      }

      return brightIdx;
    }

    function update(color) {

      const idx = computeColors(color);

      for (let i = 0; i < 6; i++) {

        const style = buttonList[i].style;
        style.backgroundColor = bColors[i];

        if (i === idx) {
          style.borderStyle = "solid";
          style.borderWidth = "0.5em";
          style.borderColor = "white";
        }
        else {
          style.border = "none";
        }
      }
    }

    function changeColor(model, idx) {

      const rgb = bColors[idx].split(/rgb\(+(.+?)\,+(.+?)\,+(.+?)\)/);
      const color = {
        color: {
          red: parseInt(rgb[1]),
          green: parseInt(rgb[2]),
          blue: parseInt(rgb[3])
        }
      };

      model.send(color);
    }
  })

  /*
  * Color picker module
  */
  .module('picker', () => {

    let node;
    let gColors = [];

    const colorTab = [// Colors, 5x3 grid
      "rgb(255,0,0)", "rgb(255,128,0)", "rgb(255,215,0)",
      "rgb(255,255,0)", "rgb(128,255,128)", "rgb(0,255,0)",
      "rgb(0,255,128)", "rgb(0,255,255)", "rgb(0,128,255)",
      "rgb(0,0,255)", "rgb(128,0,255)", "rgb(255,0,255)",
      "rgb(255,128,128)", "rgb(255,0,128)", "rgb(255,255,255)"];

    return {
      $init: ['observer', function (observer) {
        node = document.getElementById("color");

        observer.registerHandler(function (data) {
          update(data.color);
        });
      }],
      $build: ['model', function (model) {
        build(model);
      }]
    };

    //--------------------------------------------------------------
    function build(model) {

      const tablebody = document.createElement('div');
      tablebody.className = 'divTableBody';
      node.appendChild(tablebody);

      // Add buttons to panel
      for (let i = 0; i < 5; i++) {

        const tablerow = document.createElement('div');
        tablerow.className = 'divTableRow';
        tablebody.appendChild(tablerow);

        for (let j = 0; j < 3; j++) {

          const button = document.createElement('div');
          button.className = "divTableCell";
          button.style.backgroundColor = colorTab[i * 3 + j];
          button.onclick = (function (I) {
            return function () {
              changeColor(model, I);
            };
          })(i * 3 + j);

          tablerow.appendChild(button);
        }
      }
    }

    function update(color) {

      const r = color.red, g = color.green, b = color.blue;
      const brightness = Math.max(r, g, b, 1); // Avoid dividing by zero

      // Adjust color table to brightness
      for (let i = 0, len = colorTab.length; i < len; i++) {
        const rgb = colorTab[i].split(/rgb\(+(.+?)\,+(.+?)\,+(.+?)\)/);
        const r = parseInt(rgb[1]);
        const g = parseInt(rgb[2]);
        const b = parseInt(rgb[3]);
        const newR = Math.round(r * brightness / 255);
        const newG = Math.round(g * brightness / 255);
        const newB = Math.round(b * brightness / 255);
        gColors[i] = "rgb(" + newR.toString() + "," + newG.toString() + "," + newB.toString() + ")";
      }
    }

    function changeColor(model, idx) {

      const rgb = gColors[idx].split(/rgb\(+(.+?)\,+(.+?)\,+(.+?)\)/);
      const color = {
        color: {
          red: parseInt(rgb[1]),
          green: parseInt(rgb[2]),
          blue: parseInt(rgb[3])
        }
      };

      model.send(color);
    }
  })

  /*
  * Color picker module
  */
  .module('htmlPicker', () => {

    let node;

    return {
      $init: ['observer', 'model', function (observer, model) {
        node = document.getElementById("colorPicker");

        if (isSafari()) {
          node.style.display = "none";
        }
        else {
          observer.registerHandler(function (data) {
            update(data.color);
          });

          node.onchange = function () {
            changeColor(model);
          };
        }
      }]
    };

    //-------------------------------------------------------------------
    function toHex(n) {
      let hex = n.toString(16);
      while (hex.length < 2) { hex = "0" + hex; }
      return hex;
    }

    function toHexString(color) {
      const r = toHex(color.red);
      const g = toHex(color.green);
      const b = toHex(color.blue);
      return "#" + r + g + b;
    }

    function toRgbColor(colorhexs) {
      let r, g, b;
      r = parseInt(colorhexs.substr(1, 2), 16);
      g = parseInt(colorhexs.substr(3, 2), 16);
      b = parseInt(colorhexs.substr(5, 2), 16);

      return {
        color: {
          red: r,
          green: g,
          blue: b
        }
      };
    }

    function update(color) {
      node.value = toHexString(color);
    }

    function changeColor(model) {
      const color = toRgbColor(node.value);
      model.send(color);
    }

    function isSafari() {
      return navigator.userAgent.indexOf("Safari") > -1;
    }
  })

  /*
  * Info module
  */
  .module('info', ['$http', ($http) => {

    let node;

    return {
      $init: function () {
        node = document.getElementById("info");
      },
      $build: function () {
        build();
      }
    };

    //--------------------------------------------------------------------------
    function build() {

      const button = node.getElementsByClassName("button")[0];
      button.onclick = function () {

        $http.get("/info")
          .setTimeout(10000)
          .success(function (resp) {
            display(JSON.parse(resp));
          })
          .error(function (resp) {
            console.log(resp);
          })
          .timeout(function (resp) {
            console.log(resp);
          })
          .send();
      };
    }

    function display(info) {

      node.innerHTML = "";

      const table = document.createElement('table');
      table.className = "info";
      node.appendChild(table);

      for (let field in info) {
        if (info.hasOwnProperty(field)) {

          const row = document.createElement('tr');
          table.appendChild(row);

          const key = document.createElement('td');
          const value = document.createElement('td');
          row.appendChild(key);
          row.appendChild(value);

          key.innerHTML = field;
          key.className = "key";
          value.innerHTML = info[field];
          value.className = "value";
        }
      }
    }
  }])
  
  /*
  * Settings
  */
  .module('settings', ['$http', ($http) => {

    let node, timeZone, prevValue = 0;

    return {
      $init: function () {
        node = document.getElementById("settings");
      },
      $build: function () {
        build();
      }
    };

    //--------------------------------------------------------------------------
    function build() {

      const button = node.getElementsByClassName("button")[0];
      button.onclick = function () {
        render();
      };
    }

    function render() {

      node.innerHTML = "";

      timeZone = document.createElement('select');
      for(let i = -12; i <= 12; i++) {
        const option = document.createElement('option');
        option.innerText = i.toString();
        timeZone.appendChild(option);
      }

      const button = document.createElement('div');
      button.innerText = 'Update settings';
      button.className = 'button';

      button.onclick = function() {
        $http.post("/settings", JSON.stringify({
            timeZone: parseInt(timeZone.value)
          }))
          .setTimeout(10000)
          .success(function (resp) {
            update(JSON.parse(resp));
          })
          .error(function (resp) {
            console.log(resp);
          })
          .timeout(function (resp) {
            console.log(resp);
          })
          .send();
      };

      node.appendChild(timeZone);
      node.appendChild(button);

      $http.get("/settings")
          .setTimeout(10000)
          .success(function (resp) {
            update(JSON.parse(resp));
          })
          .error(function (resp) {
            console.log(resp);
          })
          .timeout(function (resp) {
            console.log(resp);
          })
          .send();
    }

    function update(settings) {
      timeZone.value = settings.timeZone;
    }
  }]);
