# Script to generate test files
param(
    [int]$Length = 1000,
    [string]$Filename = "large_test.txt"
)

$chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?"
$random = New-Object System.Random
$result = ""

for ($i = 0; $i -lt $Length; $i++) {
    $result += $chars[$random.Next(0, $chars.Length)]
}

$result | Out-File -FilePath $Filename -Encoding UTF8 -NoNewline
Write-Host "Generated $Filename with $Length characters"