# Build and test TQ project
# Run from project root: pwsh .\build.ps1

$ErrorActionPreference = "Stop"

Write-Host "====================================" -ForegroundColor Cyan
Write-Host "  TQ Project Build Script" -ForegroundColor Cyan
Write-Host "====================================" -ForegroundColor Cyan
Write-Host ""

$ROOT = $PSScriptRoot
$BUILD_SUCCESS = $true

# Build tq-core
Write-Host "[1/3] Building tq-core..." -ForegroundColor Yellow
Push-Location "$ROOT\tq-core"

if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Push-Location "build"

try {
    cmake .. 
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }
    
    cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) { throw "Build failed" }
    
    Write-Host " tq-core built successfully" -ForegroundColor Green
    
    # Run tests
    Write-Host "`n[Tests] Running tq-core tests..." -ForegroundColor Yellow
    ctest -C Release --output-on-failure
    if ($LASTEXITCODE -ne 0) { 
        Write-Host " Some tests failed" -ForegroundColor Red
        $BUILD_SUCCESS = $false
    } else {
        Write-Host " All tests passed" -ForegroundColor Green
    }
} catch {
    Write-Host " tq-core build failed: $_" -ForegroundColor Red
    $BUILD_SUCCESS = $false
}

Pop-Location
Pop-Location

# Build tq-cli
Write-Host "`n[2/3] Building tq-cli..." -ForegroundColor Yellow
Push-Location "$ROOT\tq-cli"

if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Push-Location "build"

try {
    cmake ..
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }
    
    cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) { throw "Build failed" }
    
    Write-Host " tq-cli built successfully" -ForegroundColor Green
    
    # Test CLI
    Write-Host "`n[Tests] Testing CLI..." -ForegroundColor Yellow
    $testJson = '{"name":"Alice"}'
    $result = $testJson | .\Release\tq.exe ".name" 2>$null
    if ($result -eq '"Alice"') {
        Write-Host " CLI test passed" -ForegroundColor Green
    } else {
        Write-Host " CLI test failed" -ForegroundColor Red
        $BUILD_SUCCESS = $false
    }
} catch {
    Write-Host " tq-cli build failed: $_" -ForegroundColor Red
    $BUILD_SUCCESS = $false
}

Pop-Location
Pop-Location

# Build Python bindings (optional)
Write-Host "`n[3/3] Building Python bindings..." -ForegroundColor Yellow
Push-Location "$ROOT\pytq"

try {
    $pythonInstalled = Get-Command python -ErrorAction SilentlyContinue
    if ($pythonInstalled) {
        python -m pip install -e . --quiet 2>&1 | Out-Null
        if ($LASTEXITCODE -eq 0) {
            Write-Host " Python bindings built successfully" -ForegroundColor Green
            
            # Test Python module
            $testResult = python -c "import pytq; print(pytq.query('.name', {'name': 'Alice'}))" 2>$null
            if ($testResult -match "Alice") {
                Write-Host " Python bindings test passed" -ForegroundColor Green
            } else {
                Write-Host " Python bindings test failed" -ForegroundColor Red
                $BUILD_SUCCESS = $false
            }
        } else {
            Write-Host " Python bindings build failed (optional)" -ForegroundColor Yellow
        }
    } else {
        Write-Host " Python not found, skipping bindings (optional)" -ForegroundColor Yellow
    }
} catch {
    Write-Host " Python bindings build failed (optional): $_" -ForegroundColor Yellow
}

Pop-Location

# Summary
Write-Host "`n====================================" -ForegroundColor Cyan
if ($BUILD_SUCCESS) {
    Write-Host "   BUILD SUCCESSFUL" -ForegroundColor Green
    Write-Host "====================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "  - Run CLI: .\tq-cli\build\Release\tq.exe --help"
    Write-Host "  - Run benchmarks: .\tq-core\build\tests\Release\benchmark.exe"
    Write-Host "  - See docs\ for API documentation"
} else {
    Write-Host "   BUILD FAILED" -ForegroundColor Red
    Write-Host "====================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Check error messages above for details"
    exit 1
}
