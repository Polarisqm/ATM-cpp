# ATM-cpp — Simulador de cajero automático

Aplicación de escritorio educativa escrita en **C++17** con **Qt Widgets**. Incluye una interfaz gráfica, acceso mediante PIN, depósitos, retiradas, consulta de saldo, cambio de PIN e historial de movimientos. La cuenta de demostración se guarda localmente en JSON.

> **Importante:** este proyecto es una simulación educativa, no un sistema bancario real. El PIN se almacena en texto plano en el archivo local de datos. No uses datos, contraseñas ni dinero reales.

## Funcionalidades

- Inicio de sesión con PIN de cuatro dígitos.
- Saldo inicial de demostración de `$ 1.250,00`.
- Depósitos y retiradas con validación de importes y saldo suficiente.
- Cambio de PIN con validación y confirmación.
- Historial de hasta 100 movimientos recientes.
- Persistencia local con JSON y escritura segura mediante `QSaveFile`.
- Interfaz adaptable y navegación por botones.
- CMake, compatible con Qt 5 o Qt 6 (módulo Widgets).

## Requisitos

- C++17.
- CMake 3.21 o superior.
- Qt 6 (recomendado) o Qt 5, con el módulo **Widgets**.
- Un compilador compatible (MSVC, GCC o Clang).

## Compilar y ejecutar

### Windows (Qt Creator recomendado)

1. Instala Qt desde el instalador oficial de Qt e incluye Qt Widgets y un compilador compatible (por ejemplo, MinGW o MSVC).
2. Abre `CMakeLists.txt` en Qt Creator.
3. Selecciona el kit que coincida con la versión de Qt instalada.
4. Pulsa **Configure Project**, luego **Build** y finalmente **Run**.

### Windows con CMake desde terminal

Abre una terminal de desarrollador configurada para el compilador elegido y ejecuta desde la raíz del repositorio:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Si CMake no encuentra Qt, configura `CMAKE_PREFIX_PATH` con la carpeta de instalación de Qt que contiene `lib/cmake`, por ejemplo:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build build --config Release
```

Sustituye la ruta de ejemplo por la ruta real de tu instalación. Ejecuta el `.exe` generado dentro de `build` (la ubicación exacta depende del generador y la configuración).

### Linux

Instala CMake, un compilador C++ y Qt Widgets (por ejemplo, el paquete de desarrollo Qt 6 de tu distribución). Después:

```bash
cmake -S . -B build
cmake --build build -j
./build/ATM-cpp
```

### macOS

Instala Xcode Command Line Tools, CMake y Qt 6; configura `CMAKE_PREFIX_PATH` si fuera necesario y ejecuta los mismos comandos de CMake. La ubicación de Qt depende de cómo lo hayas instalado.

## Primer inicio

- PIN inicial: **`1234`**.
- Saldo inicial: **`$ 1.250,00`**.
- El saldo, PIN e historial se guardan en la carpeta de datos de aplicación que Qt determina para el sistema operativo. El archivo se llama `account.json`.
- Cambiar el PIN se conserva al volver a abrir la aplicación.

## Estructura del proyecto

```text
ATM-cpp/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
└── src/
    ├── main.cpp
    ├── account.h
    ├── account.cpp
    ├── atmwindow.h
    └── atmwindow.cpp
```

- `main.cpp`: inicializa Qt y la aplicación.
- `account.h/.cpp`: lógica de cuenta, validaciones, movimientos y persistencia.
- `atmwindow.h/.cpp`: ventanas, formularios y acciones de la interfaz gráfica.
- `CMakeLists.txt`: configuración de compilación con CMake.

## Subirlo a GitHub

Crea un repositorio vacío llamado `ATM-cpp` en GitHub. Desde la carpeta del proyecto:

```bash
git init
git add .
git commit -m "feat: create Qt C++ ATM simulator"
git branch -M main
git remote add origin https://github.com/TU-USUARIO/ATM-cpp.git
git push -u origin main
```

Reemplaza `TU-USUARIO` por tu usuario de GitHub. No subas la carpeta `build/` ni datos personales; están excluidos por `.gitignore`.

## Próximas mejoras posibles

- Pruebas automatizadas de la lógica de cuenta.
- Cuentas múltiples y selección de usuario.
- Exportación del historial a CSV.
- Traducción de interfaz y tema claro/oscuro.
- Separar el almacenamiento de datos de la lógica de negocio.

## Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta `LICENSE`.
